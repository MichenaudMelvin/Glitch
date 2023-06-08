// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HealthComponent.h"
#include "Components/ActivableComponent.h"
#include "Components/InteractableComponent.h"
#include "FMODEvent.h"
#include "Components/BoxComponent.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "FMODAudioComponent.h"
#include "AbstractObjectif.generated.h"

UCLASS(Abstract)
class GLITCHUE_API AAbstractObjectif : public AActor{
	GENERATED_BODY()

public:
	AAbstractObjectif();

protected:
	virtual void BeginPlay() override;

	AGlitchUEGameMode* GameMode;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* MeshObjectif;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Health")
	UHealthComponent* HealthComp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Actiable")
	UActivableComponent* ActivableComp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interaction")
	UInteractableComponent* InteractableComp;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	UBoxComponent* NavModifier;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	UFMODAudioComponent* FMODAudioComp;

	UFUNCTION()
	virtual void ActiveObjectif();

	UFUNCTION()
	virtual void DesactivateObjectif();

	UFUNCTION()
	virtual void OnSwitchPhases(EPhases CurrentPhase);

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	UFMODEvent* ActivationSFX;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	UFMODEvent* DeactivationSFX;

	UFUNCTION()
	virtual void TakeDamages();

	UFUNCTION()
	virtual void HealthNull();

	UFUNCTION()
	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer);

public:
	UActivableComponent* GetActivableComp() const;

	UHealthComponent* GetHealthComp() const;
};
