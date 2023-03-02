// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/EmptyTask.h"

EBTNodeResult::Type UEmptyTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	return EBTNodeResult::Succeeded;
}
