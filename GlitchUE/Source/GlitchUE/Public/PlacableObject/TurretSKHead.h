// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/Turret.h"
#include "TurretSKHead.generated.h"


UCLASS()
class GLITCHUE_API ATurretSKHead : public ATurret{
	GENERATED_BODY()

public:
	ATurretSKHead();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh")
	USkeletalMeshComponent* TurretHead;

	float CurrentPitchRotation;

	virtual void LookAtTarget(AActor* Target) override;

	virtual void RotateToTarget(float Alpha) override;

	virtual void SetMesh() override;
};
