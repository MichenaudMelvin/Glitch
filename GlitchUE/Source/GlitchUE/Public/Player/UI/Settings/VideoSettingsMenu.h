// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Player/UI/Settings/SettingsMenu.h"
#include "VideoSettingsMenu.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UVideoSettingsMenu : public USettingsMenu{
	GENERATED_BODY()

public:
	UVideoSettingsMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativePreConstruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "SettignsElements", meta = (BindWidget))
	UCheckBox* VSyncCheckBox;

	UFUNCTION()
	void ToggleVSync(bool IsChecked);

	UPROPERTY(EditDefaultsOnly, Category = "SettignsElements", meta = (BindWidget))
	UComboBoxString* ResolutionBox;

	UFUNCTION()
	void ChangeResolution(FString SelectedItem, ESelectInfo::Type SelectionType);

	virtual void InitializeSettings() override;

	virtual void UpdateSettings() const override;
};
