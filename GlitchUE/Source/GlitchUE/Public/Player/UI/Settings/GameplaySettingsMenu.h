// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CheckBox.h"
#include "Player/UI/Settings/SettingsMenu.h"
#include "Player/UI/CustomSlider.h"
#include "GameplaySettingsMenu.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UGameplaySettingsMenu : public USettingsMenu{
	GENERATED_BODY()

public:
	UGameplaySettingsMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeDestruct() override;

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
	UCustomSlider* SensibilitySlider;

	UFUNCTION()
	void UpdateSensibilitySlider(float Value);

#pragma endregion

	virtual void InitializeSettings() override;

	virtual void UpdateSettings() const override;
};
