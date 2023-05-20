// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CheckBox.h"
#include "UI/Menu/Settings/SettingsMenu.h"
#include "UI/Custom/CustomSlider.h"
#include "GameplaySettingsMenu.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UGameplaySettingsMenu : public USettingsMenu{
	GENERATED_BODY()

public:
	UGameplaySettingsMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnInitialized() override;

#pragma region CheckBox

	UPROPERTY(EditDefaultsOnly, Category = "SettignsElements", meta = (BindWidget))
	UCheckBox* YAxisCheckBox;

	UFUNCTION()
	void ToggleYAxis(bool IsChecked);

#pragma endregion

#pragma region Sliders

	UPROPERTY(EditDefaultsOnly, Category = "SettignsElements", meta = (BindWidget))
	UCustomSlider* FOVSlider;

	UFUNCTION()
	void UpdateFOVSlider(float Value);

	UPROPERTY(EditDefaultsOnly, Category = "SettignsElements", meta = (BindWidget))
	UCustomSlider* SensitivitySlider;

	UFUNCTION()
	void UpdateSensitivitySlider(float Value);

#pragma endregion

	virtual void InitializeSettings() override;

	virtual void UpdateSettings() override;
};
