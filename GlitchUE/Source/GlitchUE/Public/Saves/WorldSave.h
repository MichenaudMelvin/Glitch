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

	UPROPERTY(BlueprintReadWrite)
	UMaterial* SaveImage;

	UPROPERTY(BlueprintReadWrite)
	TArray<FTransform> AITransformList;

	UPROPERTY(BlueprintReadWrite)
	TArray<UBlackboardData*> BlackboardList;

	UPROPERTY(BlueprintReadWrite)
	FTransform PlayerTransform;

	UPROPERTY(BlueprintReadWrite)
	FRotator PlayerCameraRotation;

	UPROPERTY(BlueprintReadWrite)
	ELevelState LevelState;

	UPROPERTY(BlueprintReadWrite)
	int LoadedTime = 0;
};
