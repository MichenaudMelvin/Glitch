// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/PlacableActor.h"
#include "Components/TimelineComponent.h"
#include "Turret.generated.h"

class AMainAICharacter;

UCLASS(Abstract)
class GLITCHUE_API ATurret : public APlacableActor{
	GENERATED_BODY()

public:
	ATurret();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* TurretPillar;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh")
	//UStaticMeshComponent* TurretHead;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vision")
	UStaticMeshComponent* TurretVision;

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	float Damages;

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	float FireRate;

	AActor* CurrentTarget;

	float CurrentYawRotation;

	FTimeline RotateTimeline;

	UCurveFloat* ZeroToOneCurve;

	UFUNCTION(BlueprintCallable)
	virtual void LookAtTarget(AActor* Target);

	UFUNCTION()
	virtual void RotateToTarget(float Alpha);

	virtual void GlitchUpgrade() override;

	virtual void SetMesh() override;

	virtual void SetData(UPlacableActorData* NewData) override;

	FRotator AILookAtRotation;
};
