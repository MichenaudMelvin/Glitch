// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/MainPlayer.h"
#include "GameFramework/Actor.h"
#include "Objectives/Catalyseur.h"
#include "Engine/DataTable.h"
#include "Spawner.h"
#include "EngineUtils.h"
#include "WaveManager.generated.h"

class AMainAICharacter;

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
	float NextWaveTimer;

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

	TSet<AMainAICharacter*> WaveAIList;

	void EnableCatalyseurs();
	
	void DisableCatalyseurs();

public:
	void StartWave();

	void EndWave();

	void AddAIToList(AMainAICharacter* AIToAdd);

	void RemoveAIFromList(AMainAICharacter* AIToRemove);

private:
	void SpawnEnemies();

	FWave* GetCurrentWaveData();

	void RefreshActiveSpawners();
};

//Wat
//https://forums.unrealengine.com/t/get-all-actors-of-class-in-c/329740/2
template<typename T>
void FindAllActors(UWorld* World, TArray<T*>& Out) {
	for (TActorIterator<T> It(World); It; ++It) {
		Out.Add(*It);
	}
}