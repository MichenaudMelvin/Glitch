// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActivableComponent.h"
#include "PlacableActor.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/BoxComponent.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "Player/TargetCameraLocation.h"
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

	UPROPERTY(EditAnywhere, Category = "Camera")
	ATargetCameraLocation* CameraTargetLocation;

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
	void SwitchPhases(EPhases NewPhases);

	APlacableActor* UnitInZone = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ConstructionZoneBlend = 0.1f;

public:
	UFUNCTION()
	void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer);

	UFUNCTION(BlueprintCallable)
	void OccupiedSlot(APlacableActor* NewUnit);

	APlacableActor* GetUnit() const;

	UFUNCTION(BlueprintCallable)
	void UnoccupiedSlot();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsSlotOccupied() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Activable")
	UActivableComponent* GetActivableComp();

	UFUNCTION()
	void DestroyCurrentUnit();

	//Call in editor cannot be in WITH_EDITORONLY_DATA directive
	UFUNCTION(CallInEditor, Category = "Camera")
	void SpawnCamera();

#if WITH_EDITORONLY_DATA
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
