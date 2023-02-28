// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/StunAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"

UStunAI::UStunAI(){
	bNotifyTick = true;

	StunTime.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UStunAI, StunTime));
}

void UStunAI::InitializeFromAsset(UBehaviorTree& Asset){
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset)){
		StunTime.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UStunAI::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){

	const UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();

	RemainingWaitTime = CurrentBlackboard->GetValue<UBlackboardKeyType_Float>(StunTime.GetSelectedKeyID());

	return EBTNodeResult::InProgress;
}

void UStunAI::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds){
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	RemainingWaitTime -= DeltaSeconds;
	
	if(RemainingWaitTime <= 0){
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
