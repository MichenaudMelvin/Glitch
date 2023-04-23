// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralDecorators/CustomDecorator.h"

UCustomDecorator::UCustomDecorator(){
	bNotifyTick = true;
}

void UCustomDecorator::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds){
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	CheckAbort(OwnerComp, NodeMemory);
}

void UCustomDecorator::CheckAbort(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const{
	if(FlowAbortMode == EBTFlowAbortMode::None){
		return;
	}

	const bool bIsOnActiveBranch = OwnerComp.IsExecutingBranch(GetMyNode(), GetChildIndex());

	bool bShouldAbort;

	if (bIsOnActiveBranch){
		bShouldAbort = (FlowAbortMode == EBTFlowAbortMode::Self || FlowAbortMode == EBTFlowAbortMode::Both) && !CalculateRawConditionValue(OwnerComp, NodeMemory);
	} else{
		bShouldAbort = (FlowAbortMode == EBTFlowAbortMode::LowerPriority || FlowAbortMode == EBTFlowAbortMode::Both) && CalculateRawConditionValue(OwnerComp, NodeMemory);
	}

	if(bShouldAbort){
			OwnerComp.RequestExecution(this);
	}
}
