// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/SwitchBehavior.h"
#include "AIController.h"
#include "AI/MainAIController.h"

USwitchBehavior::USwitchBehavior(){}

EBTNodeResult::Type USwitchBehavior::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){

	AMainAIController* CurrentController = Cast<AMainAIController>(OwnerComp.GetAIOwner());

	CurrentController->SwitchBehavior(NewBehaviorTree, NewBlackboardData);

	return EBTNodeResult::Succeeded;
}
