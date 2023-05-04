// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/MainAIData.h"
#include "PursuitDroneData.generated.h"

UCLASS()
class GLITCHUE_API UPursuitDroneData : public UMainAIData{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Crystal|Spin")
	float SpinSpeed = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Crystal|Spin")
	float SpinOffset = 90;

	UPROPERTY(EditDefaultsOnly, Category = "Crystal|Spin")
	FVector SpinScale = FVector(0.5, 0.5, 0.5);
};
