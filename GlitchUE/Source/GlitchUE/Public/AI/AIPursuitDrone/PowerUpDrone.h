// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/PlacableActor.h"
#include "PowerUpDrone.generated.h"

UCLASS()
class GLITCHUE_API APowerUpDrone : public APlacableActor{
	GENERATED_BODY()

public:
	APowerUpDrone();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* PivotPoint;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* DroneMesh;

	float SpinOffset;

	float SpinSpeed;

	float InteriorRotationOffset;

	virtual void SetData(UPlacableActorData* NewData) override;

	virtual void SetMesh() override;

	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer) override;

	UFUNCTION(BlueprintCallable, Category = "Boost")
	void BoostPlacable(APlacableActor* PlacableToBoost);

	FTimeline SpinTimeline;

	UCurveFloat* ZeroToOneCurve;

	UFUNCTION()
	void Spin(float Value);
};
