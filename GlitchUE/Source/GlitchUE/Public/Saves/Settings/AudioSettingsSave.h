// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Saves/Settings/SettingsSave.h"
#include "AudioSettingsSave.generated.h"

UCLASS()
class GLITCHUE_API UAudioSettingsSave : public USettingsSave{
	GENERATED_BODY()

public:
	UAudioSettingsSave();

	UPROPERTY(BlueprintReadWrite)
	float MasterVolume = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	float MusicVolume = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	float SFXVolume = 1.0f;
};
