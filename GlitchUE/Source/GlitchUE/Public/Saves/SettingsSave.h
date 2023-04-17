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

#pragma region Boolean

	UPROPERTY(BlueprintReadWrite)
	bool bInvertCamYAxis = false;

	UPROPERTY(BlueprintReadWrite)
	bool VSyncEnable = false;

#pragma endregion

	UPROPERTY(BlueprintReadWrite)
	float CameraFOV = 90;

	UPROPERTY(BlueprintReadWrite)
	float CameraSensibility = 1;

};