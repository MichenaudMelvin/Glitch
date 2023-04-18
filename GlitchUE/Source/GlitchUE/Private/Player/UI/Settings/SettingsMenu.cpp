// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/Settings/SettingsMenu.h"
#include "GlitchUEGameMode.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/GameUserSettings.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "Kismet/GameplayStatics.h"

void USettingsMenu::NativeOnInitialized(){
	Super::NativeOnInitialized();

	Settings = Cast<USettingsSave>(UUsefullFunctions::LoadSave(USettingsSave::StaticClass(), 0));

	GameUserSettings = UGameUserSettings::GetGameUserSettings();

	YAxisCheckBox->OnCheckStateChanged.AddDynamic(this, &USettingsMenu::ToggleYAxis);
	VSyncCheckBox->OnCheckStateChanged.AddDynamic(this, &USettingsMenu::ToggleVSync);

	FOVSlider->GetSlider()->OnValueChanged.AddDynamic(this, &USettingsMenu::UpdateFOVSlider);
	SensibilitySlider->GetSlider()->OnValueChanged.AddDynamic(this, &USettingsMenu::UpdateSensibilitySlider);

	ResetSettingsButton->OnClicked.AddDynamic(this, &USettingsMenu::ResetSettings);

	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void USettingsMenu::NativeConstruct(){
	Super::NativeConstruct();

	InitializeSettings();
}

void USettingsMenu::NativeDestruct(){
	Super::NativeDestruct();

	Settings = Cast<USettingsSave>(UUsefullFunctions::SaveToSlot(Settings, 0));
	UpdateGlobalSettings();

	if(!UGameplayStatics::GetGameMode(GetWorld())->IsA(AGlitchUEGameMode::StaticClass())){
		return;
	}

	UpdatePlayerSettings();
}

void USettingsMenu::ToggleYAxis(bool IsChecked){
	Settings->bInvertCamYAxis = IsChecked;
}

void USettingsMenu::ToggleVSync(bool IsChecked){
	Settings->VSyncEnable = IsChecked;
}

void USettingsMenu::UpdateFOVSlider(float Value){
	Settings->CameraFOV = Value;
}

void USettingsMenu::UpdateSensibilitySlider(float Value){
	Settings->CameraSensibility = Value;
}

void USettingsMenu::InitializeSettings(){

	Settings->bInvertCamYAxis ? YAxisCheckBox->SetCheckedState(ECheckBoxState::Checked) : YAxisCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	Settings->VSyncEnable ? VSyncCheckBox->SetCheckedState(ECheckBoxState::Checked) : VSyncCheckBox->SetCheckedState(ECheckBoxState::Unchecked); 

	FOVSlider->SetValue(Settings->CameraFOV);
	SensibilitySlider->SetValue(Settings->CameraSensibility);

	UpdateGlobalSettings();
}

void USettingsMenu::UpdateGlobalSettings() const{
	GameUserSettings->SetVSyncEnabled(Settings->VSyncEnable);
	GameUserSettings->ApplySettings(false);
}

void USettingsMenu::UpdatePlayerSettings() const{
	MainPlayer->GetFollowCamera()->SetFieldOfView(Settings->CameraFOV);
	MainPlayer->SetInvertAxis(Settings->bInvertCamYAxis);
	MainPlayer->SetSensibility(Settings->CameraSensibility);
}

void USettingsMenu::ResetSettings(){
	Settings = Cast<USettingsSave>(UUsefullFunctions::CreateSave(USettingsSave::StaticClass()));
	InitializeSettings();
}
