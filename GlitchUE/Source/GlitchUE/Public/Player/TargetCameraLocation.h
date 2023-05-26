// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "TargetCameraLocation.generated.h"

UCLASS()
class GLITCHUE_API ATargetCameraLocation : public AActor{
	GENERATED_BODY()

public:
	ATargetCameraLocation();

protected:
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraFOV = 150;

#if WITH_EDITORONLY_DATA
	UArrowComponent* ArrowComponent;
#endif

	virtual void OnConstruction(const FTransform& Transform) override;
};
