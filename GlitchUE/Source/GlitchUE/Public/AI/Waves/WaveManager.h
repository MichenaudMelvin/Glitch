// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlitchUEGameMode.h"
#include "AI/MainAIData.h"
#include "Player/MainPlayer.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Player/UI/TimerWidget.h"
#include "WaveManager.generated.h"

class AMainAICharacter;
class ASpawner;
class ACatalyseur;
class ANexus;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnRefreshAIList);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnStartWave, int, CurrentWave);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnEndWave, int, CurrentWave);

USTRUCT(BlueprintType)
struct FAIToSpawn{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AMainAICharacter> AIToSpawn;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int NumberToSpawn = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UMainAIData* AIData = nullptr;
};

USTRUCT(BlueprintType)
struct FWave : public FTableRowBase{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float NextWaveTimer = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bStopAtEnd = false;

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

	AGlitchUEGameMode* GameMode;

	UPROPERTY(BlueprintReadOnly)
	UTimerWidget* PlayerTimerWidget;

	UPROPERTY(BlueprintReadOnly)
	ANexus* Nexus;

	UPROPERTY()
	AAudioManager* AudioManager;

	AMainPlayer* Player;

	UPROPERTY(BlueprintReadOnly)
	int CurrentWaveNumber = 1;

	UPROPERTY(BlueprintReadOnly)
	int NumberOfWaves;

	UPROPERTY(EditAnywhere, Category = "Waves")
	UDataTable* WavesData;

	UPROPERTY(BlueprintReadOnly)
	TSet<AMainAICharacter*> WaveAIList;

	void EnableSpawners();

	void DisableSpawner();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Waves")
	FWave GetCurrentWaveData() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Waves")
	FWave GetTargetWaveData(const int Target) const;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FKOnRefreshAIList OnRefreshAIList;

	bool bIsStopped = false;

public:
	void StartWave();

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Waves")
	FKOnStartWave OnStartWave;

	void EndWave();

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Waves")
	FKOnEndWave OnEndWave;

	bool IsStopped() const;

	void AddAIToList(AMainAICharacter* AIToAdd);

	void RemoveAIFromList(const AMainAICharacter* AIToRemove);

	UFUNCTION()
	void NextWave();

	void SetWave(const int NewWave);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Waves")
	int GetCurrentWaveNumber() const;

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