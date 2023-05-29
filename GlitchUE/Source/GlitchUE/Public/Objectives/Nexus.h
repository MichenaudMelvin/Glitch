// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objectives/AbstractObjectif.h"
#include "PopcornFXEmitterComponent.h"
#include "Nexus.generated.h"

class AWaveManager;
class ACatalyseur;
class AAudioManager;
class ADissolver;
class UPlayerStats;

UCLASS()
class GLITCHUE_API ANexus : public AAbstractObjectif{
	GENERATED_BODY()

public:
	ANexus();

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	ADissolver* Dissolver;

	AAudioManager* AudioManager;

	TArray<ACatalyseur*> CatalyseursList;

	UAnimationAsset* IdleAnim;

	UPlayerStats* PlayerStatsWidget;

	virtual void TakeDamages() override;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateDissolver();

protected:
	AActor* GetFarestActivatedCatalyseur();

	virtual void ActiveObjectif() override;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UPopcornFXEmitterComponent* TechFXEmitter;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UPopcornFXEmitterComponent* MedFXEmitter;

	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer) override;
};