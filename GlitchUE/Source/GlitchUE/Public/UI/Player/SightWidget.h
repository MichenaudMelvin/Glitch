// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "SightWidget.generated.h"

UCLASS(Abstract)
class GLITCHUE_API USightWidget : public UUserWidget{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Sight", meta = (BindWidget))
	UImage* Sight;

public:
	void UpdateOpacity(const float OpacityValue) const;
};
