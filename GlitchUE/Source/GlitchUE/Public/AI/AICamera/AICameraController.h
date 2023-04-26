// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/MainAIController.h"
#include "AICameraController.generated.h"

UCLASS(Abstract)
class GLITCHUE_API AAICameraController : public AMainAIController{
	GENERATED_BODY()

public:
	AAICameraController(const FObjectInitializer& ObjectInitializer);
};
