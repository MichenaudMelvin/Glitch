// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/MainAICharacter.h"
#include "PursuitDrone.generated.h"

UCLASS()
class GLITCHUE_API APursuitDrone : public AMainAICharacter{
	GENERATED_BODY()

public:
	APursuitDrone();

protected:
	virtual void BeginPlay() override;

	// UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	// UBoxComponent* BoxComp;
	//
	// UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	// UFloatingPawnMovement* MovementComponent;
};
