 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Slider.h"
#include "UI/UIFocus.h"
#include "FocusableSlider.generated.h"

UCLASS()
class GLITCHUE_API UFocusableSlider : public USlider, public IUIFocus{
	GENERATED_BODY()

public:
	UFocusableSlider();

protected:
	FSlateBrush NormalBarBrush;

	FSlateBrush HoveredBarBrush;

	FSlateBrush NormalThumbBrush;

	FSlateBrush HoveredThumbBrush;

public:
	virtual void ReceiveFocus() override;

	virtual void UnReceiveFocus() override;
};
