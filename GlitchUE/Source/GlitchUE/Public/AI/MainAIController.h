// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MainAIController.generated.h"

UCLASS()
class GLITCHUE_API AMainAIController : public AAIController{
	GENERATED_BODY()

public:
	AMainAIController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	UBehaviorTree* BehaviorTree;

};
