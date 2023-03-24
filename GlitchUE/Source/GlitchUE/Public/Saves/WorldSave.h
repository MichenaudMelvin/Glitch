// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Saves/AbstractSave.h"
#include "GlitchUEGameMode.h"
#include "WorldSave.generated.h"

USTRUCT(BlueprintType)
struct FAIData{
	GENERATED_BODY()

public:
	UPROPERTY()
	FTransform CurrentTransform;

	UPROPERTY()
	FVector OriginalPosition;

	UPROPERTY()
	bool bIsStun;

	UPROPERTY()
	bool bInvestigate;

	UPROPERTY()
	FVector InvestigationLocation;

	UPROPERTY()
	bool bDoingExternalActions;

	UPROPERTY()
	bool bReceiveAlert;

	// should be FPatrolData
	UPROPERTY()
	int CurrentIndex;
};

USTRUCT(BlueprintType)
struct FPatrolData : public FAIData{
	GENERATED_BODY()

public:
	// UPROPERTY()
	// int CurrentIndex;
};

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
	TMap<FString, FAIData> AIDataList;

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	FTransform PlayerTransform;

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	FRotator PlayerCameraRotation;

	UPROPERTY(BlueprintReadWrite, Category = "Mark")
	FTransform MarkTransform;

	UPROPERTY(BlueprintReadWrite, Category = "Mark")
	bool bIsMarkPlaced;

	UPROPERTY(BlueprintReadWrite, Category = "World")
	ELevelState LevelState;

	UPROPERTY(BlueprintReadWrite, Category = "World")
	float GlitchValue;
};
