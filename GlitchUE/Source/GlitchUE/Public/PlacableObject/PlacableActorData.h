// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Assets/PopcornFXEffect.h"
#include "Engine/DataAsset.h"
#include "PlacableActorData.generated.h"

class APlacableActor;

UCLASS()
class GLITCHUE_API UPlacableActorData : public UPrimaryDataAsset{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Name")
	FName Name;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stats")
	float AttackRange;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stats")
	float AttackRate;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stats")
	float Damages;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Golds")
	int Cost;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Golds")
	UPlacableActorData* NextUpgrade;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mesh")
	TArray<UStreamableRenderAsset*> MeshList;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mesh")
	UStaticMesh* FullMesh;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Golds")
	bool HasEnoughGolds(const int PlayerGolds) const;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Glitch")
	float GlitchUpgradeDuration;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Glitch")
	float GlitchGaugeValueOnDestruct;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Glitch")
	float GlitchDamages;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Glitch")
	float GlitchAttackRate;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Glitch")
	float GlitchAttackRange;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "FX")
	UPopcornFXEffect* AttackFX;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation")
	UAnimSequenceBase* AttackAnimation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Class")
	TSubclassOf<APlacableActor> ClassToSpawn;
};
