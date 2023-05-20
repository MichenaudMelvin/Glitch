// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Saves/WorldSave.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "StealthSave.generated.h"

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
class GLITCHUE_API UStealthSave : public UWorldSave{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "AI")
	TMap<FString, FAIData> AIDataList;

	UPROPERTY(BlueprintReadWrite, Category = "World")
	ELevelState LevelState;

	UPROPERTY(BlueprintReadWrite, Category = "World")
	bool bIsStealthTimeRunning;

	UPROPERTY(BlueprintReadWrite, Category = "World")
	float RemainingStealthTime;
};
