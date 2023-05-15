// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PopcornFXEmitterComponent.h"
#include "Objectives/AbstractObjectif.h"
#include "FMODEvent.h"
#include "Nexus.generated.h"

class AWaveManager;
class ACatalyseur;
class AAudioManager;
class ADissolver;

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

	virtual void TakeDamages() override;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateDissolver();

protected:
	AActor* GetFarestActivatedCatalyseur();

	virtual void ActiveObjectif() override;

	UPROPERTY()
	UPopcornFXEmitterComponent* TechFXEmitter;

	UPopcornFXEffect* TechFX;

	UPROPERTY()
	UPopcornFXEmitterComponent* MedFXEmitter;

	UPopcornFXEffect* MedFX;

	UPROPERTY(EditDefaultsOnly)
	UFMODEvent* ActivationSFX;

	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer) override;
};