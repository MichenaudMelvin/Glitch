// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FocusableSlider.h"
#include "Engine/Font.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "UI/UIFocus.h"
#include "CustomSlider.generated.h"

UCLASS()
class GLITCHUE_API UCustomSlider : public UHorizontalBox, public IUIFocus{
	GENERATED_BODY()

public:
	UCustomSlider();

protected:
	virtual void PostLoad() override;

	FSlateFontInfo NumberFont;

	UPROPERTY(EditDefaultsOnly, Category = "Slider", meta = (BindWidget))
	UHorizontalBox* SliderContainer;

	UPROPERTY(EditDefaultsOnly, Category = "Slider", meta = (BindWidget))
	UFocusableSlider* Slider;

	UPROPERTY(EditAnywhere, Category = "Slider")
	float SliderMinValue;

	UPROPERTY(EditAnywhere, Category = "Slider")
	float SliderMaxValue;

	UPROPERTY(EditAnywhere, Category = "Slider")
	float IntegralDigits = 3;

	UPROPERTY(EditAnywhere, Category = "Slider")
	float FractionalDigits = 0;

	UPROPERTY(EditAnywhere, Category = "Slider")
	float StepSize = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Slider", meta = (BindWidget))
	UTextBlock* SliderValue;

	UFUNCTION()
	void UpdateText(float Value);

public:
	virtual void ReceiveFocus() override;

	virtual void UnReceiveFocus() override;

	void SetValue(const float Value);

	USlider* GetSlider() const;
};
