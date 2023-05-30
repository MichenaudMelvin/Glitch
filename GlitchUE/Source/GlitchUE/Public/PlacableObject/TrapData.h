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
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stats")
	float TrapDuration;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Behavior")
	ETrapEffect TrapEffect = ETrapEffect::Burned;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Behavior")
	float TrapEffectDuration;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Behavior")
	float ReactivationTimer;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Behavior", meta = (EditCondition = "TrapEffect == ETrapEffect::Burned"))
	float EffectTickRate;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Behavior", meta = (EditCondition = "TrapEffect == ETrapEffect::Burned"))
	float EffectDamages;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Behavior", meta = (EditCondition = "TrapEffect == ETrapEffect::SlowedDown"))
	float SlowSpeed;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "FX")
	UPopcornFXEffect* IdleFX;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "FX")
	UPopcornFXEffect* EffectFX;
};
