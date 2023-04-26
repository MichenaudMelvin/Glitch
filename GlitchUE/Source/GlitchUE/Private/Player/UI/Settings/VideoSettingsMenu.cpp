// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/Settings/VideoSettingsMenu.h"
#include "Saves/Settings/VideoSettingsSave.h"

UVideoSettingsMenu::UVideoSettingsMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
	SettingClass = UVideoSettingsSave::StaticClass();

	SettingCategory = FText::FromString("Video");
}

void UVideoSettingsMenu::NativeOnInitialized(){
	Super::NativeOnInitialized();

	VSyncCheckBox->OnCheckStateChanged.AddDynamic(this, &UVideoSettingsMenu::ToggleVSync);
}

void UVideoSettingsMenu::ToggleVSync(bool IsChecked){
	Cast<UVideoSettingsSave>(Settings)->VSyncEnable = IsChecked;
}

void UVideoSettingsMenu::InitializeSettings(){

	Cast<UVideoSettingsSave>(Settings)->VSyncEnable ? VSyncCheckBox->SetCheckedState(ECheckBoxState::Checked) : VSyncCheckBox->SetCheckedState(ECheckBoxState::Unchecked); 

	Super::InitializeSettings();
}

void UVideoSettingsMenu::UpdateSettings() const{
	Super::UpdateSettings();

	GameUserSettings->SetVSyncEnabled(Cast<UVideoSettingsSave>(Settings)->VSyncEnable);
	GameUserSettings->ApplySettings(false);
}
