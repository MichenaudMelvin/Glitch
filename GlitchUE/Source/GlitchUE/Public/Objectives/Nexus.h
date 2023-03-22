// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FX/Dissolver.h"
#include "PopcornFXEmitterComponent.h"
#include "Objectives/AbstractObjectif.h"
#include "Nexus.generated.h"

class AWaveManager;
class ACatalyseur;

UCLASS()
class GLITCHUE_API ANexus : public AAbstractObjectif{
	GENERATED_BODY()

public:
	ANexus();

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	ADissolver* Dissolver;

	TArray<ACatalyseur*> CatalyseursList;

	virtual void TakeDamages() override;

	UFUNCTION(BlueprintCallable)
	void UpdateDissolver();

	AActor* GetFarestActivatedCatalyseur();

	virtual void ActiveObjectif() override;

	UPROPERTY()
	UPopcornFXEmitterComponent* TechFXEmitter;

	UPopcornFXEffect* TechFX;

	UPROPERTY()
	UPopcornFXEmitterComponent* MedFXEmitter;

	UPopcornFXEffect* MedFX;

	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer) override;
};