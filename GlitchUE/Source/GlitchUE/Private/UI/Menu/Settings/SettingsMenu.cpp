// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/Settings/SettingsMenu.h"
#include "GameFramework/GameUserSettings.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "Kismet/GameplayStatics.h"

void USettingsMenu::NativeOnInitialized(){
	Super::NativeOnInitialized();

	GameUserSettings = UGameUserSettings::GetGameUserSettings();

	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	Gamemode = Cast<AMainGamemode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void USettingsMenu::NativeConstruct(){
	Super::NativeConstruct();

	Settings = Cast<USettingsSave>(UUsefullFunctions::LoadSave(SettingClass, 0));

	InitializeSettings();
}

void USettingsMenu::InitializeSettings(){}

void USettingsMenu::UpdateSettings(){
	Settings = Cast<USettingsSave>(UUsefullFunctions::SaveToSlot(Settings, 0));
}

void USettingsMenu::ResetSettings(){
	Settings = Cast<USettingsSave>(UUsefullFunctions::CreateSave(SettingClass, 0));
	InitializeSettings();
	UpdateSettings();
}

FText USettingsMenu::GetSettingCategory() const{
	return SettingCategory;
}

TArray<UWidget*> USettingsMenu::GetFocusableWidgets() const{
	return FocusableWidgets;
}
