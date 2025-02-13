// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Saves/Settings/SettingsSave.h"
#include "VideoSettingsSave.generated.h"

UCLASS()
class GLITCHUE_API UVideoSettingsSave : public USettingsSave{
	GENERATED_BODY()

public:
	UVideoSettingsSave();

	UPROPERTY(BlueprintReadWrite)
	bool bVSyncEnable = false;

	UPROPERTY(BlueprintReadWrite)
	bool bVolumetricLighting = false;

	UPROPERTY()
	TEnumAsByte<EWindowMode::Type> WindowMode = EWindowMode::WindowedFullscreen;

	UPROPERTY(BlueprintReadWrite)
	FIntPoint Resolution = FIntPoint(1920, 1080);
};
