// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/MainAIController.h"
#include "PursuitDroneController.generated.h"

UCLASS(Abstract)
class GLITCHUE_API APursuitDroneController : public AMainAIController{
	GENERATED_BODY()

public:
	APursuitDroneController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	virtual void InitializeAIFromStart() override;

	virtual void InitializeAI(const FAIData NewData) override;

	virtual void SwitchBehavior(UBehaviorTree* NewBehaviorTree, UBlackboardData* NewBlackboardData) override;

	virtual FAIData SaveAI() override;

	TArray<ACatalyseur*> CatalyseurList;

	virtual TArray<ACatalyseur*> GetCatalyseurList() const override;
};
