// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActivableComponent.h"
#include "PlacableActor.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/ArrowComponent.h"
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

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* TechMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	UInteractableComponent* InteractableComponent;

#if WITH_EDITORONLY_DATA
	UArrowComponent* CameraWheelDirection;
#endif

	UAnimationAsset* ActivationAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UActivableComponent* ActivableComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBoxComponent* NavObstacle;

	UPROPERTY()
	UPopcornFXEmitterComponent* ConstructionFXEmitter;

	AGlitchUEGameMode* GameMode;

	UPopcornFXEffect* ConstructionEffect;

	AMainPlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EState InitialState;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (MakeEditWidget = true))
	FTransform CameraWheelTransform = FTransform(FRotator(-90, 0, 0), FVector(0, 0, 500), FVector(1, 1, 1));

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

#if WITH_EDITORONLY_DATA
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
