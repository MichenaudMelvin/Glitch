// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Saves/AbstractSave.h"
#include "BehaviorTree/BlackboardData.h"
#include "GlitchUEGameMode.h"
#include "WorldSave.generated.h"

UCLASS()
class GLITCHUE_API UWorldSave : public UAbstractSave{
	GENERATED_BODY()

public:
	UWorldSave();

	UPROPERTY(BlueprintReadWrite, Category = "Save")
	UMaterial* SaveImage;

	UPROPERTY(BlueprintReadWrite, Category = "Save")
	int LoadedTime = 0; 

	UPROPERTY(BlueprintReadWrite, Category = "AI")
	TArray<FTransform> AITransformList;

	UPROPERTY(BlueprintReadWrite, Category = "AI")
	TArray<UBlackboardData*> BlackboardList;

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	FTransform PlayerTransform;

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	FRotator PlayerCameraRotation;

	UPROPERTY(BlueprintReadWrite, Category = "World")
	ELevelState LevelState;

	UPROPERTY(BlueprintReadWrite, Category = "World")
	float GlitchValue;
};
