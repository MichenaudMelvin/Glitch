// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/MainPlayer.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "WaveManager.generated.h"

class AMainAICharacter;
class ASpawner;
class ACatalyseur;
class ANexus;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnRefreshAIList);

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

	TSet<ASpawner*> SpawnerList;

	TArray<ASpawner*> ActiveSpawnerList;

	UPROPERTY(BlueprintReadOnly)
	ANexus* Nexus;

	AMainPlayer* Player;

	UPROPERTY(BlueprintReadOnly)
	int CurrentWaveNumber = 1;

	UPROPERTY(BlueprintReadOnly)
	int NumberOfWaves;

	UDataTable* WavesData;

	UPROPERTY(BlueprintReadOnly)
	TSet<AMainAICharacter*> WaveAIList;

	void EnableCatalyseurs();
	
	void DisableCatalyseurs();

	void EnableSpawners();

	void DisableSpawner();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FWave GetCurrentWaveData();

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FKOnRefreshAIList OnRefreshAIList;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Waves")
	void StartWave();
	virtual void StartWave_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Waves")
	void EndWave();
	virtual void EndWave_Implementation();

	void AddAIToList(AMainAICharacter* AIToAdd);

	void RemoveAIFromList(AMainAICharacter* AIToRemove);

	// DEBUG ONLY
	void SetWave(int NewWave);

	int GetCurrentWaveNumber();

private:
	void SpawnEnemies();

	bool HaveTheSpawnerFinished();
};

//https://forums.unrealengine.com/t/get-all-actors-of-class-in-c/329740/2
template<typename T>
void FindAllActors(UWorld* World, TArray<T*>& Out) {
	for (TActorIterator<T> It(World); It; ++It) {
		Out.Add(*It);
	}
}