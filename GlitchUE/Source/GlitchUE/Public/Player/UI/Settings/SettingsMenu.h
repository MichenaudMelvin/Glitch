// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CheckBox.h"
#include "Player/MainPlayer.h"
#include "Player/UI/CustomSlider.h"
#include "Player/UI/SubWidget.h"
#include "Saves/SettingsSave.h"
#include "SettingsMenu.generated.h"

UCLASS()
class GLITCHUE_API USettingsMenu : public USubWidget{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeDestruct() override;

	USettingsSave* Settings;

	UGameUserSettings* GameUserSettings;

	AMainPlayer* MainPlayer;

	UPROPERTY(EditDefaultsOnly, Category = "SettignsElements", meta = (BindWidget))
	UButton* ResetSettingsButton;

#pragma region CheckBox

	UPROPERTY(EditDefaultsOnly, Category = "SettignsElements", meta = (BindWidget))
	UCheckBox* YAxisCheckBox;

	UFUNCTION()
	void ToggleYAxis(bool IsChecked);

	UPROPERTY(EditDefaultsOnly, Category = "SettignsElements", meta = (BindWidget))
	UCheckBox* VSyncCheckBox;

	UFUNCTION()
	void ToggleVSync(bool IsChecked);

#pragma endregion

#pragma region Sliders

#pragma endregion

	UPROPERTY(EditDefaultsOnly, Category = "SettignsElements", meta = (BindWidget))
	UCustomSlider* FOVSlider;

	UFUNCTION()
	void UpdateFOVSlider(float Value);

	void InitializeSettings();

	void UpdateGlobalSettings();

	void UpdatePlayerSettings();

	UFUNCTION()
	void ResetSettings();
};
