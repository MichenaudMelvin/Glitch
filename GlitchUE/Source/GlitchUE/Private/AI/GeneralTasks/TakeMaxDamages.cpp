// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/TakeMaxDamages.h"
#include "AIController.h"
#include "AI/MainAICharacter.h"

UTakeMaxDamages::UTakeMaxDamages(){}

EBTNodeResult::Type UTakeMaxDamages::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){

	Cast<AMainAICharacter>(OwnerComp.GetAIOwner()->GetPawn())->GetHealthComp()->TakeMaxDamages();

	return EBTNodeResult::Succeeded;
}
