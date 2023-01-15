// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/Turret.h"
#include "TurretSMHead.generated.h"

UCLASS()
class GLITCHUE_API ATurretSMHead : public ATurret{
	GENERATED_BODY()

public:
	ATurretSMHead();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* TurretHead;

	float CurrentPitchRotation;

	virtual void LookAtTarget(AActor* Target) override;

	virtual void RotateToTarget(float Alpha) override;

	virtual void SetMesh() override;
};
