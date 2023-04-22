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
		CalculateRawConditionValue(OwnerComp, NodeMemory);
		return;
	}

	if(FlowAbortMode != EBTFlowAbortMode::None){
		if(CalculateRawConditionValue(OwnerComp, NodeMemory)){
			OwnerComp.RequestExecution(this);
		}
	}
}
