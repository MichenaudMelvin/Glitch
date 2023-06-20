// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu//Settings/AudioSettingsMenu.h"
#include "Helpers/FunctionsLibrary/UsefulFunctions.h"
#include "Saves/Settings/AudioSettingsSave.h"

UAudioSettingsMenu::UAudioSettingsMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
	SettingClass = UAudioSettingsSave::StaticClass();

	SettingCategory = FText::FromString("Audio");
}

void UAudioSettingsMenu::NativeOnInitialized(){
	Super::NativeOnInitialized();

	MasterVolumeSlider->GetSlider()->OnValueChanged.AddDynamic(this, &UAudioSettingsMenu::UpdateMasterVolumeSlider);
	MusicVolumeSlider->GetSlider()->OnValueChanged.AddDynamic(this, &UAudioSettingsMenu::UpdateMusicVolumeSlider);
	SFXVolumeSlider->GetSlider()->OnValueChanged.AddDynamic(this, &UAudioSettingsMenu::UpdateSFXVolumeSlider);

	FocusableWidgets.Add(MasterVolumeSlider->GetSlider());
	FocusableWidgets.Add(MusicVolumeSlider->GetSlider());
	FocusableWidgets.Add(SFXVolumeSlider->GetSlider());
}

void UAudioSettingsMenu::UpdateMasterVolumeSlider(float Value){
	Cast<UAudioSettingsSave>(Settings)->MasterVolume = Value;
	UpdateSettings();
}

void UAudioSettingsMenu::UpdateMusicVolumeSlider(float Value){
	Cast<UAudioSettingsSave>(Settings)->MusicVolume = Value;
	UpdateSettings();
}

void UAudioSettingsMenu::UpdateSFXVolumeSlider(float Value){
	Cast<UAudioSettingsSave>(Settings)->SFXVolume = Value;
	// Update Settings aren't needed
}

void UAudioSettingsMenu::InitializeSettings(){
	const UAudioSettingsSave* CastedSettings = Cast<UAudioSettingsSave>(Settings);

	MasterVolumeSlider->SetValue(CastedSettings->MasterVolume);
	MusicVolumeSlider->SetValue(CastedSettings->MusicVolume);
	SFXVolumeSlider->SetValue(CastedSettings->SFXVolume);

	Super::InitializeSettings();
}

void UAudioSettingsMenu::UpdateSettings(){
	Super::UpdateSettings();

	Gamemode->UpdateAudioSettings(Cast<UAudioSettingsSave>(Settings));
}
