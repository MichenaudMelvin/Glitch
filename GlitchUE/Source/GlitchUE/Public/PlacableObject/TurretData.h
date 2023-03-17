// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/PlacableActorData.h"
#include "Turret.h"
#include "TurretData.generated.h"

UCLASS()
class GLITCHUE_API UTurretData : public UPlacableActorData{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Behavior")
	bool CanSeeThroughWalls = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Behavior")
	FFocusMethod FocusMethod;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Glitch")
	float GlitchFireRate;
};
