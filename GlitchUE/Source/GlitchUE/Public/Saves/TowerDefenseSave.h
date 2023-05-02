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
};

UCLASS()
class GLITCHUE_API UTowerDefenseSave : public UWorldSave{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Player")
	int PlayerGolds;

	UPROPERTY(BlueprintReadWrite, Category = "Waves")
	int CurrentWave;

	UPROPERTY(BlueprintReadWrite, Category = "Placables")
	TArray<FPlacableActorSaveData> PlacableDataList;

};
