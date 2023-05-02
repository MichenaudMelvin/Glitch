// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/Settings/SettingsMenu.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "Kismet/GameplayStatics.h"

void USettingsMenu::NativeOnInitialized(){
	Super::NativeOnInitialized();

	Settings = Cast<USettingsSave>(UUsefullFunctions::LoadSave(SettingClass, 0));

	GameUserSettings = UGameUserSettings::GetGameUserSettings();

	ResetSettingsButton->OnClicked.AddDynamic(this, &USettingsMenu::ResetSettings);

	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	Gamemode = Cast<AMainGamemode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void USettingsMenu::NativeConstruct(){
	Super::NativeConstruct();

	InitializeSettings();
}

void USettingsMenu::NativeDestruct(){
	Super::NativeDestruct();

	Settings = Cast<USettingsSave>(UUsefullFunctions::SaveToSlot(Settings, 0));
	UpdateSettings();
}

void USettingsMenu::InitializeSettings(){}

void USettingsMenu::UpdateSettings() const{}

void USettingsMenu::ResetSettings(){
	Settings = Cast<USettingsSave>(UUsefullFunctions::CreateSave(SettingClass, 0));
	InitializeSettings();
}

FText USettingsMenu::GetSettingCategory() const{
	return SettingCategory;
}
