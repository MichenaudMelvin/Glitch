// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objectives/AbstractObjectif.h"
#include "PopcornFXEmitterComponent.h"
#include "Nexus.generated.h"

class AWaveManager;
class ACatalyseur;
class ADissolver;
class UPlayerStats;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnFreeEnoughCatalyseur);

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

	UAnimationAsset* IdleAnim;

	UPlayerStats* PlayerStatsWidget;

	/**
	 * @brief considered as an health update 
	 */
	virtual void TakeDamages() override;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateDissolver();

	void SetCanInteractWithNexus(const bool bCanInteract) const;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates")
	FKOnFreeEnoughCatalyseur OnFreeEnoughCatalyseur;

protected:
	AActor* GetFarestActivatedCatalyseur();

	virtual void ActiveObjectif() override;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UPopcornFXEmitterComponent* TechFXEmitter;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UPopcornFXEmitterComponent* MedFXEmitter;

	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer) override;

	UFUNCTION()
	void OnEndInteract(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer);
};