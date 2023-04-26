// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/MainPlayer.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Saves/Settings/SettingsSave.h"
#include "GameFramework/GameUserSettings.h"
#include "SettingsMenu.generated.h"

UCLASS(Abstract)
class GLITCHUE_API USettingsMenu : public UUserWidget{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "Name")
	FText SettingCategory;

	UPROPERTY()
	USettingsSave* Settings;

	TSubclassOf<USettingsSave> SettingClass;

	UGameUserSettings* GameUserSettings;

	AMainPlayer* MainPlayer;

	UPROPERTY(EditDefaultsOnly, Category = "SettignsElements", meta = (BindWidget))
	UButton* ResetSettingsButton;

	virtual void InitializeSettings();

	virtual void UpdateSettings() const;

	UFUNCTION()
	virtual void ResetSettings();

public:
	FText GetSettingCategory() const;
};
