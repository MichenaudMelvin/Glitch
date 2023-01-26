// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objectives/AbstractObjectif.h"
#include "Nexus.h"
#include "PlacableObject/ConstructionZone.h"
#include "Catalyseur.generated.h"

class ASpawner;

USTRUCT(BlueprintType)
struct FStateAtWave{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EnableAtWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DisableAtWave;
};

UCLASS()
class GLITCHUE_API ACatalyseur : public AAbstractObjectif{
	GENERATED_BODY()

public:
	ACatalyseur();

protected:
	virtual void BeginPlay() override;

	virtual void ActiveObjectif() override;

	virtual void DesactivateObjectif() override;

	virtual void HealthNull() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh")
	USkeletalMeshComponent* TECHMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves", meta = (ExposeOnSpawn = "true"))
	FStateAtWave StateAtWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves", meta = (ExposeOnSpawn = "true"))
	TArray<ASpawner*> SpawnerList;

	ANexus* Nexus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionZone", meta = (ExposeOnSpawn = "true"))
	TArray<AConstructionZone*> ConstructionZoneList;

public:
	FStateAtWave GetStateAtWave();
};
