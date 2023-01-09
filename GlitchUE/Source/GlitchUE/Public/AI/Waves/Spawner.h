// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActivableComponent.h"
#include "AI/MainAICharacter.h"
#include "Spawner.generated.h"

UCLASS()
class GLITCHUE_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawner();

protected:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SpawnerMesh;

	UActivableComponent* ActivableComp;

public:
	void Spawn(int numberToSpawn, TSubclassOf<AMainAICharacter> AIToSpawn);

	UActivableComponent* GetActivableComp();
};
