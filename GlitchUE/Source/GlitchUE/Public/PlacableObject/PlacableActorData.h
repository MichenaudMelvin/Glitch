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
#if WITH_EDITOR
	virtual void PostLoad() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Name")
	FName Name;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Name")
	FText Description;

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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mesh")
	FLinearColor CrystalColor = FLinearColor(0, 0, 0, 1);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Golds")
	bool HasEnoughGolds(const int PlayerGolds) const;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Glitch")
	float GlitchUpgradeDuration;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Glitch")
	float GlitchGaugeValueOnDestruct;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Glitch")
	float GlitchDamages;

	// Damages + GlitchDamages
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Transient, Category = "Glitch")
	float RealGlitchDamages;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Glitch")
	float GlitchAttackRate;

	// AttackRate + GlitchAttackRate
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Transient, Category = "Glitch")
	float RealGlitchAttackRate;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Glitch")
	float GlitchAttackRange;

	// AttackRange + GlitchAttackRange
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Transient, Category = "Glitch")
	float RealGlitchAttackRange;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "BoostDrone")
	float BoostDroneDamages;

	// Damages + BoostDroneDamages
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Transient, Category = "BoostDrone")
	float RealBoostDroneDamages;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "BoostDrone")
	float BoostDroneAttackRate;

	// AttackRate + BoostDroneAttackRate
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Transient, Category = "BoostDrone")
	float RealBoostDroneAttackRate;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "BoostDrone")
	float BoostDroneAttackRange;

	// AttackRange + BoostDroneAttackRange
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Transient, Category = "BoostDrone")
	float RealBoostDroneAttackRange;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "FX")
	UPopcornFXEffect* AttackFX;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "FX")
	FVector AttackFXOffset = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "FX")
	UMaterialInstance* AppearanceMaterial;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation")
	UAnimSequenceBase* AttackAnimation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation")
	UAnimSequenceBase* IdleAnimation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Class")
	TSubclassOf<APlacableActor> ClassToSpawn;
};
