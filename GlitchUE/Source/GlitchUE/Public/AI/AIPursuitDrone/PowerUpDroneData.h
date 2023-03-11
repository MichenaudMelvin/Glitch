// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/PlacableActorData.h"
#include "PowerUpDroneData.generated.h"

UCLASS()
class GLITCHUE_API UPowerUpDroneData : public UPlacableActorData{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Behavior")
	float SpinOffset;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Behavior")
	float SpinSpeed;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Behavior")
	float InteriorRotationOffset;
};
