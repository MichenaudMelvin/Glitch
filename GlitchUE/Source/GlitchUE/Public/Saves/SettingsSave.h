// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Saves/AbstractSave.h"
#include "SettingsSave.generated.h"

UCLASS()
class GLITCHUE_API USettingsSave : public UAbstractSave{
	GENERATED_BODY()

public:
	USettingsSave();

	UPROPERTY(BlueprintReadWrite)
	bool bInvertCamYAxis = false;

	UPROPERTY(BlueprintReadWrite)
	float CamreaFOV = 90;

};
