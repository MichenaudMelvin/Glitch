// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/SelfDestroy.h"

USelfDestroy::USelfDestroy(){}

EBTNodeResult::Type USelfDestroy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){

	OwnerComp.GetOwner()->Destroy();
	
	return EBTNodeResult::Succeeded;
}
