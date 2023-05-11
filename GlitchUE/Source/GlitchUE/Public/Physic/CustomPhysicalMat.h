// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "CustomPhysicalMat.generated.h"

UCLASS()
class GLITCHUE_API UCustomPhysicalMat : public UPhysicalMaterial{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (UIMin = "0.0", UIMax = "1.0"), Category = "Hearing")
	float SoundResistance = 1;

public:
	float GetSoundResistance() const;
};
