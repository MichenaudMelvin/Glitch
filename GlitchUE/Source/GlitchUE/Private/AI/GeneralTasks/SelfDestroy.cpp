// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/SelfDestroy.h"
#include "AIController.h"

USelfDestroy::USelfDestroy(){}

EBTNodeResult::Type USelfDestroy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){

	OwnerComp.GetAIOwner()->GetPawn()->Destroy();
	
	return EBTNodeResult::Succeeded;
}
