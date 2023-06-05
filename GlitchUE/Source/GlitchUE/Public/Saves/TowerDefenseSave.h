// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Saves/WorldSave.h"
#include "TowerDefenseSave.generated.h"

class APlacableActor;
class UPlacableActorData;

USTRUCT(BlueprintType)
struct FPlacableActorSaveData{
	GENERATED_BODY()

public:
	UPROPERTY()
	TSubclassOf<APlacableActor> PlacableActorClass;

	UPROPERTY()
	FTransform ActorTransform;

	UPROPERTY()
	UPlacableActorData* CurrentPlacableData = nullptr;

	UPROPERTY()
	bool bHasDrone = false;

	UPROPERTY()
	FString DroneName;
};

USTRUCT(BlueprintType)
struct FCatalyseurData{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsActivated = false;

	UPROPERTY()
	bool bWasActivatedInStealthPhase = false;

	UPROPERTY()
	float DesactivationTimer = 0;

	UPROPERTY()
	float Health = 0;
};

UCLASS()
class GLITCHUE_API UTowerDefenseSave : public UWorldSave{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Waves")
	int CurrentWave;

	UPROPERTY(BlueprintReadWrite, Category = "Placables")
	TArray<FPlacableActorSaveData> PlacableDataList;

	UPROPERTY(BlueprintReadWrite, Category = "Catalyseur")
	TMap<FString, FCatalyseurData> CatalyseurDataList;

	UPROPERTY(BlueprintReadWrite, Category = "Catalyseur")
	int CurrentActivatedCatalyseurNumber;

	UPROPERTY(BlueprintReadWrite, Category = "Nexus")
	float NexusHealth;
};
