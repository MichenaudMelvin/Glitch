// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/MainAIController.h"
#include "PatrolController.generated.h"

UCLASS(Abstract)
class GLITCHUE_API APatrolController : public AMainAIController{
	GENERATED_BODY()

public:
	APatrolController(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeAIFromStart() override;

	virtual FAIData SaveAI() override;

	virtual void InitializeAI(const FAIData NewData) override;
};
