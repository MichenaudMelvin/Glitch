// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/MainAIController.h"
#include "Objectives/Nexus.h"
#include "FocusAIController.generated.h"

UCLASS()
class GLITCHUE_API AFocusAIController : public AMainAIController{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	float AttackDistance = 250;

	ANexus* Nexus;
};
