// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/MainPlayer.h"
#include "Blueprint/UserWidget.h"
#include "Saves/Settings/SettingsSave.h"
#include "Gamemodes/MainGamemode.h"
#include "SettingsMenu.generated.h"

UCLASS(Abstract)
class GLITCHUE_API USettingsMenu : public UUserWidget{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "Name")
	FText SettingCategory;

	UPROPERTY()
	USettingsSave* Settings;

	TSubclassOf<USettingsSave> SettingClass;

	UGameUserSettings* GameUserSettings;

	AMainGamemode* Gamemode;

	AMainPlayer* MainPlayer;

	virtual void InitializeSettings();

public:
	UFUNCTION()
	virtual void UpdateSettings();

	UFUNCTION()
	virtual void ResetSettings();

	FText GetSettingCategory() const;
};
