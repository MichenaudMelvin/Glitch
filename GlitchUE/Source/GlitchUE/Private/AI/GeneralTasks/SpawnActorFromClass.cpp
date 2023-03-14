// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/SpawnActorFromClass.h"
#include "AIController.h"

USpawnActorFromClass::USpawnActorFromClass(){
	
}

EBTNodeResult::Type USpawnActorFromClass::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){

	const APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();

	GetWorld()->SpawnActor<AActor>(Class, CurrentPawn->GetActorLocation(), CurrentPawn->GetActorRotation());

	return EBTNodeResult::Succeeded;
}
