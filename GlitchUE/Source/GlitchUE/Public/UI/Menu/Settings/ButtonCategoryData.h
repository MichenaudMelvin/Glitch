// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsContainerMenu.h"
#include "ButtonCategoryData.generated.h"

UCLASS()
class GLITCHUE_API UButtonCategoryData : public UObject{
	GENERATED_BODY()

public:
	USettingsContainerMenu* SettingsContainer;

	USettingsMenu* WidgetToShow;

	FText Category;
};
