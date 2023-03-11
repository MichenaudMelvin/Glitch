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

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* DroneMesh;

	virtual void SetMesh() override;

	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer) override;
};
