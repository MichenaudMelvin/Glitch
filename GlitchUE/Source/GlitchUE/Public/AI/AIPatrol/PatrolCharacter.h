// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PatrolPoint.h"
#include "AI/MainAICharacter.h"
#include "PatrolCharacter.generated.h"

UCLASS()
class GLITCHUE_API APatrolCharacter : public AMainAICharacter{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Behavior")
	TArray<APatrolPoint*> PatrolPointsList;

public:
	const TArray<APatrolPoint*> GetPatrolPointList();
	
};
