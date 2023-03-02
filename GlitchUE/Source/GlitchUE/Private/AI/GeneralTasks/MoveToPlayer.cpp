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

	MoveToPlayer();

	return EBTNodeResult::InProgress;
}

void UMoveToPlayer::MoveToPlayer(){
	if(bUseSight){
		CheckIfPlayerIsInSight();
	}
	MoveTask();

	GetWorld()->GetTimerManager().SetTimer(MoveToTimer, [&]() {
		MoveToPlayer();
	}, 0.2f, false);
}

void UMoveToPlayer::MoveTask(){
	const FVector StartLocation = CurrentBlackboard->GetValue<UBlackboardKeyType_Vector>(PlayerLocation.GetSelectedKeyID());
	FVector EndLocation = StartLocation;
	EndLocation.Z -= 250;
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(CurrentPawn);

	FHitResult HitResult;

	FVector TargetLocation;

	if(UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLocation, EndLocation, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true, FLinearColor::Red, FLinearColor::Green, 5.0f)){
		TargetLocation = HitResult.ImpactPoint;
	} else{
		TargetLocation = StartLocation;
	}

	const EPathFollowingRequestResult::Type Result = CurrentController->MoveToLocation(TargetLocation, 5, false, true, false, false, UNavAreaCostAsOne::StaticClass(), true);

	switch (Result) {
		case EPathFollowingRequestResult::Failed:
			EndTask(EBTNodeResult::Aborted);
		break;
		case EPathFollowingRequestResult::AlreadyAtGoal:
			EndTask(EBTNodeResult::Aborted);
		break;
		case EPathFollowingRequestResult::RequestSuccessful:
			return;
		break;
	}

	EndTask(EBTNodeResult::Aborted);
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
		if(CurrentPawn->GetActorLocation().Equals(CurrentBlackboard->GetValue<UBlackboardKeyType_Vector>(PlayerLocation.GetSelectedKeyID()), 50)){
			if(!GetWorld()->GetTimerManager().IsTimerActive(TimerLoosePlayer)){
				
				GetWorld()->GetTimerManager().SetTimer(TimerLoosePlayer, [&]() {

					CurrentBlackboard->SetValue<UBlackboardKeyType_Object>(Player.GetSelectedKeyID(), nullptr);

					EndTask(EBTNodeResult::Aborted);
				}, 3, false);
			}
		}
	}
}

void UMoveToPlayer::EndTask(const EBTNodeResult::Type TaskResult){
	GetWorld()->GetTimerManager().ClearTimer(MoveToTimer);
	GetWorld()->GetTimerManager().ClearTimer(TimerPlayerInSight);
	GetWorld()->GetTimerManager().ClearTimer(TimerLoosePlayer);
	FinishLatentTask(*BTOwnerComp, TaskResult);
}
