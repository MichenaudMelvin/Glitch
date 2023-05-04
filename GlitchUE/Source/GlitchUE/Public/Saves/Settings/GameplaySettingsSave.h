// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsSave.h"
#include "GameplaySettingsSave.generated.h"

UCLASS()
class GLITCHUE_API UGameplaySettingsSave : public USettingsSave{
	GENERATED_BODY()

public:
	UGameplaySettingsSave();

	UPROPERTY(BlueprintReadWrite)
	bool bInvertCamYAxis = false;

	UPROPERTY(BlueprintReadWrite)
	float CameraFOV = 90;

	UPROPERTY(BlueprintReadWrite)
	float CameraSensibility = 1;
};
