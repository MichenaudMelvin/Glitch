// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/MainPlayer.h"
#include "GameFramework/Actor.h"
#include "Objectives/Catalyseur.h"
#include "Engine/DataTable.h"
#include "AI/MainAICharacter.h"
#include "Spawner.h"
#include "WaveManager.generated.h"

UENUM(BlueprintType)
enum class EWaveEvent : uint8 {
	ExecuteAtStart,
	ExecuteAtEnd,
};

USTRUCT(BlueprintType)
struct FWaveGolds{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int Golds;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EWaveEvent WaveEvent;
};

USTRUCT(BlueprintType)
struct FAIToSpawn{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AMainAICharacter> AIToSpawn;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int NumberToSpawn;
};

USTRUCT(BlueprintType)
struct FWave : public FTableRowBase{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float WaveDuration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FWaveGolds GivenGolds;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bStopAtEnd;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FAIToSpawn> AIToSpawnList;
};

UCLASS()
class GLITCHUE_API AWaveManager : public AActor{
	GENERATED_BODY()
	
public:	
	AWaveManager();

protected:
	virtual void BeginPlay() override;

	TSet<ACatalyseur*> CatalyseursList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves", meta = (ExposeOnSpawn = "true"))
	TSet<ASpawner*> SpawnerList;

	TArray<ASpawner*> ActiveSpawnerList;

	AMainPlayer* Player;

	int CurrentWaveNumber = 0;

	int NumberOfWaves;

	UDataTable* WavesData;

	void EnableCatalyseurs();
	
	void DisableCatalyseurs();

public:
	void StartWave();

	void EndWave();

private:
	void SpawnEnemies();

	FWave* GetCurrentWaveData();

	void RefreshActiveSpawners();
};
