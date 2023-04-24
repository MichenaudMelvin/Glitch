// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActivableComponent.h"
#include "AI/MainAICharacter.h"
#include "GlitchUEGameMode.h"
#include "Objectives/Catalyseur.h"
#include "Spawner.generated.h"

UCLASS()
class GLITCHUE_API ASpawner : public AActor{
	GENERATED_BODY()
	
public:	
	ASpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* SpawnerMesh;

	UPROPERTY(EditDefaultsOnly)
	UActivableComponent* ActivableComp;

	AWaveManager* WaveManager;

	void SpawnAI();

	AGlitchUEGameMode* Gamemode;

	FActorSpawnParameters ActorSpawnParameters;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float SpawnDelay = 1.0f;

	int NumberOfAISpawn;

	int CurrentNumberOfAISpawned;

	UMainAIData* CurrentAIData;

	TSubclassOf<AMainAICharacter> CurrentAITOSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves", meta = (ExposeOnSpawn = "true"))
	FStateAtWave StateAtWave;

	UFUNCTION()
	void ActivateSpawner();

	UFUNCTION()
	void DesactivateSpawner();

public:
	void BeginSpawn(const int NumberToSpawn, const TSubclassOf<AMainAICharacter> AIToSpawn, UMainAIData* AIData);

	UActivableComponent* GetActivableComp();

	bool AnyAILeftToSpawn() const;

	FStateAtWave GetStateAtWave() const;

	//DEBUG ONLY
	void ForceEndSpawn();
};
