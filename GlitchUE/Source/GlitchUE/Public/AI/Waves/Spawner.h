// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActivableComponent.h"
#include "AI/MainAICharacter.h"
#include "WaveManager.h"
#include "GlitchUEGameMode.h"
#include "Spawner.generated.h"

UCLASS()
class GLITCHUE_API ASpawner : public AActor{
	GENERATED_BODY()
	
public:	
	ASpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UStaticMeshComponent* SpawnerMesh;

	UPROPERTY(EditAnywhere)
	UActivableComponent* ActivableComp;

	AWaveManager* WaveManager;

	void SpawnAI();

	AGlitchUEGameMode* Gamemode;
	
	FActorSpawnParameters ActorSpawnParameters;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float SpawnDelay = 1.0f;

	int NumberOfAISpawn;

	int CurrentNumberOfAISpawned;

	TSubclassOf<AMainAICharacter> CurrentAITOSpawn;

public:
	void BeginSpawn(int NumberToSpawn, TSubclassOf<AMainAICharacter> AIToSpawn);

	UActivableComponent* GetActivableComp();

	bool AnyAILeftToSpawn();

	//DEBUG ONLY
	void ForceEndSpawn();
};
