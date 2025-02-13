// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "LevelElements/AbstractDoor.h"
#include "BasicDoor.generated.h"

UCLASS()
class GLITCHUE_API ABasicDoor : public AAbstractDoor{
	GENERATED_BODY()

public:
	ABasicDoor();

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	UBoxComponent* AIObstacle;

	UPROPERTY(EditDefaultsOnly, Category = "Door")
	UStaticMeshComponent* UpDoorFrameMed;

	UPROPERTY(EditDefaultsOnly, Category = "Door")
	UStaticMeshComponent* UpDoorFrameTech;

	UPROPERTY(EditAnywhere, Category = "Door", meta = (ClampMin = 0, ClampMax = 5))
	int UpDoorFrameIndex = 0;

	UPROPERTY(VisibleAnywhere, Category = "Door")
	TArray<UStaticMesh*> UpDoorFrameMedMeshes;

	UPROPERTY(VisibleAnywhere, Category = "Door")
	TArray<UMaterialInstance*> UpDoorFrameTechMaterial;

	int RotationFactor = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Door|Time")
	float OpenDoorTimeWhenPlayerSprint = 0.25;

	UPROPERTY(EditDefaultsOnly, Category = "Door|Time")
	float OpenDoorTimeWhenPlayerSneak = 1;

	FRotator TargetOpenRotation = FRotator(0, 90, 0);

	virtual void OpenDoor() override;

	virtual void CloseDoor() override;

	virtual void OpenDoorUpdate(float Alpha) override;

	virtual void OpenDoorEditor() override;

	virtual void CloseDoorEditor() override;

	void SelectDoorRotationFactor();

	/**
	 * @brief 
	 * @return Return witch a character forward vector
	 */
	FVector SelectDoorParameters();

public:
	// call by the gamemode
	virtual void InitializeDoor(const FBasicDoorData Data);

	FBasicDoorData SaveDoor() const;
};
