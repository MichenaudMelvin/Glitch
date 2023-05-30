// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "UI/UIFocus.h"
#include "CustomSlider.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UCustomSlider : public UUserWidget, public IUIFocus{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(EditDefaultsOnly, Category = "Slider", meta = (BindWidget))
	UHorizontalBox* SliderContainer;

	UPROPERTY(EditDefaultsOnly, Category = "Slider", meta = (BindWidget))
	USlider* Slider;

	UPROPERTY(EditAnywhere, Category = "Slider")
	float SliderMinValue;

	UPROPERTY(EditAnywhere, Category = "Slider")
	float SliderMaxValue;

	UPROPERTY(EditAnywhere, Category = "Slider")
	float IntegralDigits = 3;

	UPROPERTY(EditAnywhere, Category = "Slider")
	float FractionalDigits = 0;

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
