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
	FTransform CurrentTransform = FTransform::Identity;

	UPROPERTY()
	FVector OriginalPosition = FVector::ZeroVector;

	UPROPERTY()
	FRotator OriginalRotation = FRotator::ZeroRotator;

	UPROPERTY()
	bool bIsStun = false;

	UPROPERTY()
	bool bInvestigate = false;

	UPROPERTY()
	FVector InvestigationLocation = FVector::ZeroVector;

	UPROPERTY()
	bool bDoingExternalActions = false;

	UPROPERTY()
	bool bReceiveAlert = false;

	// should be in FPatrolData
	UPROPERTY()
	int CurrentIndex = 0;

	// should be in FPatrolData
	UPROPERTY()
	bool bHearSound = false;

	// should be in FPatrolData
	UPROPERTY()
	FVector HearingLocation = FVector::ZeroVector;

	// should be in FPatrolData
	UPROPERTY()
	bool bIsMovingToHearingLocation = false;

	// should be in FDroneData
	UPROPERTY()
	bool bIsDocked = false;
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

	UPROPERTY(BlueprintReadWrite, Category = "Inhibiteur")
	TMap<FString, bool> InhibiteurStateList;
};
