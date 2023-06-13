// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FMODEvent.h"
#include "PlayFMODSound.generated.h"

UCLASS()
class GLITCHUE_API UPlayFMODSound : public UBTTaskNode{
	GENERATED_BODY()

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Sound")
	UFMODEvent* SoundToPlay;

	UPROPERTY(EditAnywhere, Category = "Sound")
	bool bIs2DSound = false;
};
