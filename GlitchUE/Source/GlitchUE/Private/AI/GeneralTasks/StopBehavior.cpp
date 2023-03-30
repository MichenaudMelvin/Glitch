// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/StopBehavior.h"
#include "AI/MainAIController.h"

UStopBehavior::UStopBehavior(){}

EBTNodeResult::Type UStopBehavior::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	const AMainAIController* CurrentController = Cast<AMainAIController>(OwnerComp.GetAIOwner());

	CurrentController->GetBrainComponent()->StopLogic("Stop Behavior");
	CurrentController->OnStopBehavior.Broadcast();

	return EBTNodeResult::Succeeded;
}
