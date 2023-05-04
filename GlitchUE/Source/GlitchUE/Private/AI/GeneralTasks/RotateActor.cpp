// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/RotateActor.h"
#include "AIController.h"
#include "AI/AICamera/AICameraPawn.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "GameFramework/Pawn.h"

URotateActor::URotateActor(){
	bNotifyTick = true;

	CameraBehavior.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(URotateActor, CameraBehavior), StaticEnum<ECameraBehavior>());
	SpeedFactor.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(URotateActor, SpeedFactor));
	MaxRotation.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(URotateActor, MaxRotation));
	MinRotation.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(URotateActor, MinRotation));
}

void URotateActor::InitializeFromAsset(UBehaviorTree& Asset){
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset)){
		CameraBehavior.ResolveSelectedKey(*BBAsset);
		SpeedFactor.ResolveSelectedKey(*BBAsset);
		MaxRotation.ResolveSelectedKey(*BBAsset);
		MinRotation.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type URotateActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	const UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();

	const ECameraBehavior Behavior = static_cast<ECameraBehavior>(CurrentBlackboard->GetValue<UBlackboardKeyType_Enum>(CameraBehavior.GetSelectedKeyID()));
	OnRotate.Clear();

	switch (Behavior){
		case ECameraBehavior::Static:

			return EBTNodeResult::Succeeded;
		case ECameraBehavior::Clamped:
			OnRotate.AddDynamic(this, &URotateActor::ClampRotation);

			return EBTNodeResult::InProgress;
		case ECameraBehavior::FullRotation:
			OnRotate.AddDynamic(this, &URotateActor::FullRotation);

			return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Aborted;
}

void URotateActor::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds){
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* CameraPawn = OwnerComp.GetAIOwner()->GetPawn();

	OnRotate.Broadcast(CameraPawn, &OwnerComp);
}

void URotateActor::ClampRotation(APawn* PawnToRotate, UBehaviorTreeComponent* OwnerComp){
	const UBlackboardComponent* CurrentBlackboard = OwnerComp->GetBlackboardComponent();
	FRotator AddedRotation;

	if(bPositivRotation){
		AddedRotation = FRotator(0, 1, 0) * CurrentBlackboard->GetValue<UBlackboardKeyType_Float>(SpeedFactor.GetSelectedKeyID());
		PawnToRotate->AddActorLocalRotation(AddedRotation);

		if(PawnToRotate->GetActorRotation().Yaw >= CurrentBlackboard->GetValue<UBlackboardKeyType_Float>(MaxRotation.GetSelectedKeyID())){
			bPositivRotation = !bPositivRotation;
			FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
		}
	} else{
		AddedRotation = FRotator(0, -1, 0) * CurrentBlackboard->GetValue<UBlackboardKeyType_Float>(SpeedFactor.GetSelectedKeyID());
		PawnToRotate->AddActorLocalRotation(AddedRotation);

		if(PawnToRotate->GetActorRotation().Yaw <= CurrentBlackboard->GetValue<UBlackboardKeyType_Float>(MinRotation.GetSelectedKeyID())){
			bPositivRotation = true;
			FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

void URotateActor::FullRotation(APawn* PawnToRotate, UBehaviorTreeComponent* OwnerComp){
	const UBlackboardComponent* CurrentBlackboard = OwnerComp->GetBlackboardComponent();

	const FRotator AddedRotation = FRotator(0, -1, 0) * CurrentBlackboard->GetValue<UBlackboardKeyType_Float>(SpeedFactor.GetSelectedKeyID());
	PawnToRotate->AddActorLocalRotation(AddedRotation);
}