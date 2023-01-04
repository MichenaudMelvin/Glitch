// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/Catalyseur.h"
#include "WaveManager.generated.h"

UCLASS()
class GLITCHUE_API AWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AWaveManager();

protected:
	virtual void BeginPlay() override;

	TArray<ACatalyseur*> CatalyseursList;
	
	AMainPlayer* Player;

	float WaveDuration;

	int CurrentWaveNumber;

	int WaveNumber;

	int GivenGoldsAtWave;

public:
	void StartWave();
};
