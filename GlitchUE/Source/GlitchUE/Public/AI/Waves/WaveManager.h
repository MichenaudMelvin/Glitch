// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/MainPlayer.h"
#include "GameFramework/Actor.h"
#include "Objectives/Catalyseur.h"
#include "Engine/DataTable.h"
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
	UPROPERTY()
	int Golds;

	UPROPERTY()
	EWaveEvent WaveEvent;
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
};



UCLASS()
class GLITCHUE_API AWaveManager : public AActor{
	GENERATED_BODY()
	
public:	
	AWaveManager();

protected:
	virtual void BeginPlay() override;

	TSet<ACatalyseur*> CatalyseursList;

	AMainPlayer* Player;

	int CurrentWaveNumber = 0;

	int NumberOfWaves;

	//int WaveNumber;

	UDataTable* WavesData;

public:
	void StartWave();

	void EndWave();

private:
	FWave* GetCurrentWaveData();
};
