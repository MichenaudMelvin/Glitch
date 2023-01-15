// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/PlacableActorData.h"
#include "TurretData.generated.h"

UCLASS()
class GLITCHUE_API UTurretData : public UPlacableActorData{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Damages")
	float Damages;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Damages")
	float TurretRadius;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Damages")
	float FireRate;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Glitch")
	float UpgradedGlitchDamages;
};
