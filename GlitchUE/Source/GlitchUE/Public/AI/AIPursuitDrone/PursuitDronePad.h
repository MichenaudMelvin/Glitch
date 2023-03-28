// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "PursuitDronePad.generated.h"

UCLASS()
class GLITCHUE_API APursuitDronePad : public ASkeletalMeshActor{
	GENERATED_BODY()

public:
	APursuitDronePad();

	void PlayAnim(const bool bReverseAnim = false) const;

protected:
	UAnimationAsset* PadAnim;
};
