// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "CustomSlider.generated.h"

UCLASS()
class GLITCHUE_API UCustomSlider : public UUserWidget{
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

	UPROPERTY(EditDefaultsOnly, Category = "Slider", meta = (BindWidget))
	UTextBlock* SliderValue;

	UFUNCTION()
	void UpdateText(float Value);

public:
	void SetValue(const float Value);

	USlider* GetSlider() const;
};
