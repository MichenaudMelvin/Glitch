// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/PlacableActorData.h"
#include "Trap.h"
#include "TrapData.generated.h"

UCLASS()
class GLITCHUE_API UTrapData : public UPlacableActorData{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Damages")
	float TrapDamages;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Damages")
	float TrapDuration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Damages")
	float TrapAttackRate;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Behavior")
	ETrapEffect TrapEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Behavior")
	float TrapEffectDuration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Glitch")
	float UpgradedGlitchAttackRate;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FX")
	UPopcornFXEffect* IdleFX;
};
