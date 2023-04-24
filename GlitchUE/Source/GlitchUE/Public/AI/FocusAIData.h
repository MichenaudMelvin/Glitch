// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/MainAIData.h"
#include "FocusAIData.generated.h"

UCLASS()
class GLITCHUE_API UFocusAIData : public UMainAIData{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Stats|Behavior");
	float AttackDistance = 250;
};
