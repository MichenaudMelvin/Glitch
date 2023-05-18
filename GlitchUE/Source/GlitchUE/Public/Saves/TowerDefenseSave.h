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

UCLASS()
class GLITCHUE_API UTowerDefenseSave : public UWorldSave{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Dissolver")
	float DissolverRadius;

	UPROPERTY(BlueprintReadWrite, Category = "Waves")
	int CurrentWave;

	UPROPERTY(BlueprintReadWrite, Category = "Placables")
	TArray<FPlacableActorSaveData> PlacableDataList;

};
