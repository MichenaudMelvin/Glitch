// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "PursuitDronePad.generated.h"

class APursuitDrone;

UCLASS(Abstract)
class GLITCHUE_API APursuitDronePad : public ASkeletalMeshActor{
	GENERATED_BODY()

public:
	APursuitDronePad();

	void PlayAnim(const bool bReverseAnim = false) const;

	void SetCurrentDrone(APursuitDrone* NewDrone);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* MedMesh;

	UAnimationAsset* PadAnim;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, Category = "Drone")
	APursuitDrone* CurrentDrone;
};
