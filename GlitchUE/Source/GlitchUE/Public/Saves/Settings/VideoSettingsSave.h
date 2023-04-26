// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Saves/Settings/SettingsSave.h"
#include "VideoSettingsSave.generated.h"

UCLASS()
class GLITCHUE_API UVideoSettingsSave : public USettingsSave{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool VSyncEnable = false;
};
