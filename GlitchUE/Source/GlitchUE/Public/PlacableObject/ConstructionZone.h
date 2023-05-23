// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActivableComponent.h"
#include "PlacableActor.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/BoxComponent.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "ConstructionZone.generated.h"

UCLASS()
class GLITCHUE_API AConstructionZone : public ASkeletalMeshActor{
	GENERATED_BODY()

public:
	AConstructionZone();

	USkeletalMeshComponent* GetTechMesh() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* TechMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	UInteractableComponent* InteractableComponent;

	UAnimationAsset* ActivationAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UActivableComponent* ActivableComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBoxComponent* NavObstacle;

	UPROPERTY()
	UPopcornFXEmitterComponent* ConstructionFXEmitter;

	AGlitchUEGameMode* GameMode;

	UPopcornFXEffect* ConstructionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EState InitialState;

	UFUNCTION()
	void ActiveObjectif();

	UFUNCTION()
	void DesactivateObjectif();

	UFUNCTION()
	void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer);

	UFUNCTION()
	void SwitchPhases(EPhases NewPhases);

	APlacableActor* UnitInZone = nullptr;

public:
	UFUNCTION(BlueprintCallable)
	void OccupiedSlot(APlacableActor* NewUnit);

	UFUNCTION(BlueprintCallable)
	void UnoccupiedSlot();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsSlotOccupied() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Activable")
	UActivableComponent* GetActivableComp();

	UFUNCTION()
	void DestroyCurrentUnit();

#if WITH_EDITORONLY_DATA
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
