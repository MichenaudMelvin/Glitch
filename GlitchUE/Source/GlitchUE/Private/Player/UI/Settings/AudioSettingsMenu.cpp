// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/Settings/AudioSettingsMenu.h"
#include "FMODBlueprintStatics.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "Saves/Settings/AudioSettingsSave.h"

UAudioSettingsMenu::UAudioSettingsMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
	SettingClass = UAudioSettingsSave::StaticClass();

	SettingCategory = FText::FromString("Audio");

	static ConstructorHelpers::FObjectFinder<UFMODVCA> MasterVCA(TEXT("/Game/FMOD/VCAs/Master"));
	check(MasterVCA.Succeeded());

	MasterBank = MasterVCA.Object;

	static ConstructorHelpers::FObjectFinder<UFMODVCA> MusicVCA(TEXT("/Game/FMOD/VCAs/Music"));
	check(MusicVCA.Succeeded());

	MusicBank = MusicVCA.Object;

	static ConstructorHelpers::FObjectFinder<UFMODVCA> SFXVCA(TEXT("/Game/FMOD/VCAs/SFX"));
	check(SFXVCA.Succeeded());

	SFXBank = SFXVCA.Object;
}

void UAudioSettingsMenu::NativeOnInitialized(){
	Super::NativeOnInitialized();

	MasterVolumeSlider->GetSlider()->OnValueChanged.AddDynamic(this, &UAudioSettingsMenu::UpdateMasterVolumeSlider);
	MusicVolumeSlider->GetSlider()->OnValueChanged.AddDynamic(this, &UAudioSettingsMenu::UpdateMusicVolumeSlider);
	SFXVolumeSlider->GetSlider()->OnValueChanged.AddDynamic(this, &UAudioSettingsMenu::UpdateSFXVolumeSlider);
}

void UAudioSettingsMenu::UpdateMasterVolumeSlider(float Value){
	Cast<UAudioSettingsSave>(Settings)->MasterVolume = Value;
}

void UAudioSettingsMenu::UpdateMusicVolumeSlider(float Value){
	Cast<UAudioSettingsSave>(Settings)->MusicVolume = Value;
}

void UAudioSettingsMenu::UpdateSFXVolumeSlider(float Value){
	Cast<UAudioSettingsSave>(Settings)->SFXVolume = Value;
}

void UAudioSettingsMenu::InitializeSettings(){
	const UAudioSettingsSave* CastedSettings = Cast<UAudioSettingsSave>(Settings);

	MasterVolumeSlider->SetValue(CastedSettings->MasterVolume);
	MusicVolumeSlider->SetValue(CastedSettings->MusicVolume);
	SFXVolumeSlider->SetValue(CastedSettings->SFXVolume);

	Super::InitializeSettings();
}

void UAudioSettingsMenu::UpdateSettings() const{
	Super::UpdateSettings();

	const UAudioSettingsSave* CastedSettings = Cast<UAudioSettingsSave>(Settings);

	UFMODBlueprintStatics::VCASetVolume(MasterBank, CastedSettings->MasterVolume);
	UFMODBlueprintStatics::VCASetVolume(MusicBank, CastedSettings->MusicVolume);
	UFMODBlueprintStatics::VCASetVolume(SFXBank, CastedSettings->SFXVolume);
}
