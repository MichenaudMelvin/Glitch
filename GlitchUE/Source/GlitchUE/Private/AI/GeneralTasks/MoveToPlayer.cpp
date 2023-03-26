// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/GeneralTasks/MoveToPlayer.h"
#include "AIController.h"
#include "AI/Navigation/NavAreaCostAsOne.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Kismet/KismetSystemLibrary.h"

UMoveToPlayer::UMoveToPlayer(){
	Player.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UMoveToPlayer, Player), AActor::StaticClass());
	PlayerLocation.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UMoveToPlayer, PlayerLocation));
}

void UMoveToPlayer::InitializeFromAsset(UBehaviorTree& Asset){
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset)){
		Player.ResolveSelectedKey(*BBAsset);
		PlayerLocation.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UMoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	BTOwnerComp = &OwnerComp;

	CurrentBlackboard = BTOwnerComp->GetBlackboardComponent();

	CurrentController = BTOwnerComp->GetAIOwner();
	CurrentPawn = CurrentController->GetPawn();
	SightComponent = Cast<USightComponent>(CurrentPawn->GetComponentByClass(USightComponent::StaticClass()));
	TargetActor = Cast<AActor>(CurrentBlackboard->GetValue<UBlackboardKeyType_Object>(Player.GetSelectedKeyID()));

	GetWorld()->GetTimerManager().SetTimer(MoveToTimer, this, &UMoveToPlayer::MoveToPlayer, 0.2f, true);

	FinishLatentTask(*BTOwnerComp, EBTNodeResult::InProgress);
	return EBTNodeResult::InProgress;
}

void UMoveToPlayer::MoveToPlayer(){
	if(bUseSight){
		CheckIfPlayerIsInSight();
	}

	MoveTask();
}

void UMoveToPlayer::MoveTask(){
	const FVector StartLocation = CurrentBlackboard->GetValue<UBlackboardKeyType_Vector>(PlayerLocation.GetSelectedKeyID());
	FVector EndLocation = StartLocation;
	EndLocation.Z -= 250;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(CurrentPawn);

	FHitResult HitResult;

	FVector TargetLocation;

	if(UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLocation, EndLocation, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true, FLinearColor::Red, FLinearColor::Green, 5.0f)){
		TargetLocation = HitResult.ImpactPoint;
	} else{
		TargetLocation = StartLocation;
	}

	const EPathFollowingRequestResult::Type Result = CurrentController->MoveToLocation(TargetLocation, 5, false, true, false, false, UNavAreaCostAsOne::StaticClass(), true);

	switch (Result) {
		case EPathFollowingRequestResult::Failed:
			EndTask(EBTNodeResult::Failed);
		break;
		case EPathFollowingRequestResult::AlreadyAtGoal:
			EndTask(EBTNodeResult::Failed);
		break;
		case EPathFollowingRequestResult::RequestSuccessful:
			return;
	}

	EndTask(EBTNodeResult::Failed);
}

void UMoveToPlayer::CheckIfPlayerIsInSight(){
	if(SightComponent->IsPlayerInSight()){
		GetWorld()->GetTimerManager().ClearTimer(TimerPlayerInSight);
	} else{
		if(!GetWorld()->GetTimerManager().IsTimerActive(TimerPlayerInSight)){
			MoveTask();

			GetWorld()->GetTimerManager().SetTimer(TimerPlayerInSight, [&]() {
				FindPlayer();
			}, 0.1f, true);
		}
	}
}

void UMoveToPlayer::FindPlayer(){
	if(SightComponent->IsPlayerInSight()){
		GetWorld()->GetTimerManager().ClearTimer(TimerPlayerInSight);
	} else{
		if(!CurrentPawn->GetActorLocation().Equals(CurrentBlackboard->GetValue<UBlackboardKeyType_Vector>(PlayerLocation.GetSelectedKeyID()), 50)){

			if(!GetWorld()->GetTimerManager().IsTimerActive(TimerLoosePlayer)){

				GetWorld()->GetTimerManager().SetTimer(TimerLoosePlayer, [&]() {
					EndTask(EBTNodeResult::Failed);
				}, LooseTime, false);
			}
		}
	}
}

void UMoveToPlayer::EndTask(const EBTNodeResult::Type TaskResult){
	GetWorld()->GetTimerManager().ClearTimer(MoveToTimer);
	GetWorld()->GetTimerManager().ClearTimer(TimerPlayerInSight);
	GetWorld()->GetTimerManager().ClearTimer(TimerLoosePlayer);

	// pour éviter que le pawn se déplace meme après la fin de la task
	CurrentController->MoveToLocation(CurrentPawn->GetActorLocation());

	FinishLatentTask(*BTOwnerComp, TaskResult);
}
