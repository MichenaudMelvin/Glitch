// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/PlayFMODSound.h"
#include "AIController.h"
#include "FMODBlueprintStatics.h"

EBTNodeResult::Type UPlayFMODSound::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){

	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();

	bIs2DSound ? UFMODBlueprintStatics::PlayEvent2D(OwnerPawn, SoundToPlay, true) : UFMODBlueprintStatics::PlayEventAtLocation(OwnerPawn, SoundToPlay, OwnerPawn->GetActorTransform(), true);

	return EBTNodeResult::Succeeded;
}
