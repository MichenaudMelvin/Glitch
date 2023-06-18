// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/MainAIData.h"
#include "Player/MainPlayer.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "UI/Gameplay/AdditionalMessage.h"
#include "UI/Gameplay/TimerWidget.h"
#include "UI/Gameplay/Tchat/Tchat.h"
#include "WaveManager.generated.h"

class AMainAICharacter;
class ASpawner;
class ACatalyseur;
class ANexus;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnRefreshAIList);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnFinishAllWaves);

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

	UFUNCTION()
	void OnCleanWorld(UWorld* World, bool bSessionEnded, bool bCleanupResources);

	TSet<ACatalyseur*> CatalyseursList;

	TSet<ASpawner*> SpawnerList;

	TArray<ASpawner*> ActiveSpawnerList;

	AGlitchUEGameMode* GameMode;

	UPROPERTY()
	UTimerWidget* PlayerTimerWidget;

	UPROPERTY()
	UAdditionalMessage* PlayerMessageWidget;

	UPROPERTY()
	UTchat* PlayerTchatWidget;

	UPROPERTY()
	UPlayerStats* PlayerStatsWidget;

	void UpdatePlayerObjectives();

	FTimerHandle ObjectiveDelayTimerHandle;

	/**
	 * @brief show up only on prepare time
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Objectives")
	FString PrepareObjectiveText = "Prepare your defenses";

	/**
	 * @brief show up only on prepare time
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Objectives")
	FString PrepareAdditionalText = "Start building towers near the nexus";

	void WriteWhatTheNextWaveContain(const FWave TargetWave, const int TargetWaveIndex);

	UFUNCTION()
	void WriteMessages();

	UPROPERTY(EditDefaultsOnly, Category = "Tchat")
	FLinearColor TchatSpeakerColor = FLinearColor::Yellow;

	UPROPERTY(EditDefaultsOnly, Category = "Tchat")
	FString Speaker = "I.V.A.N.";

	UPROPERTY(EditDefaultsOnly, Category = "Tchat")
	float MessagesDelay = 0.5f;

	FTimerHandle MessageDelayTimerHandle;

	FWave TchatTargetWave;

	int TchatIndex = 0;

	UPROPERTY()
	ANexus* Nexus;

	UPROPERTY()
	AAudioManager* AudioManager;

	AMainPlayer* Player;

	int CurrentWaveNumber = 1;

	int NumberOfWaves;

	/**
	 * @brief Timer in seconds
	 */
	UPROPERTY(EditAnywhere, Category = "Waves")
	float PrepareTime = 60;

	FTimerHandle PrepareTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Waves")
	UDataTable* WavesData;

	UPROPERTY(BlueprintReadOnly)
	TSet<AMainAICharacter*> WaveAIList;

	void EnableSpawners(const int TargetWave);

	void DisableSpawner(const int TargetWave);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Waves")
	FWave GetCurrentWaveData() const;

	FWave GetNextWaveData() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Waves")
	FWave GetTargetWaveData(const int Target) const;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AI")
	FKOnRefreshAIList OnRefreshAIList;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AI")
	FKOnFinishAllWaves OnFinishAllWaves;

	bool bIsStopped = false;

	int GetActiveSpawnersAtWave(const int TargetWave) const;

public:
	void StartPrepareTimer();

	UFUNCTION()
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

	void ForceNextWave();

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