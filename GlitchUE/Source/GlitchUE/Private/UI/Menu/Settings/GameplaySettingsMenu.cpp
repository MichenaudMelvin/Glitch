// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/Settings/GameplaySettingsMenu.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "Saves/Settings/GameplaySettingsSave.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

UGameplaySettingsMenu::UGameplaySettingsMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
	SettingClass = UGameplaySettingsSave::StaticClass();

	SettingCategory = FText::FromString("Gameplay");
}

void UGameplaySettingsMenu::NativeOnInitialized(){
	Super::NativeOnInitialized();

	YAxisCheckBox->OnCheckStateChanged.AddDynamic(this, &UGameplaySettingsMenu::ToggleYAxis);
	AutoSlideCameraCheckBox->OnCheckStateChanged.AddDynamic(this, &UGameplaySettingsMenu::ToggleAutoSlideCamera);

	FOVSlider->GetSlider()->OnValueChanged.AddDynamic(this, &UGameplaySettingsMenu::UpdateFOVSlider);
	SensitivitySlider->GetSlider()->OnValueChanged.AddDynamic(this, &UGameplaySettingsMenu::UpdateSensitivitySlider);

	FocusableWidgets.Add(YAxisCheckBox);
	FocusableWidgets.Add(AutoSlideCameraCheckBox);
	FocusableWidgets.Add(FOVSlider->GetSlider());
	FocusableWidgets.Add(SensitivitySlider->GetSlider());
}

void UGameplaySettingsMenu::ToggleYAxis(bool IsChecked){
	Cast<UGameplaySettingsSave>(Settings)->bInvertCamYAxis = IsChecked;
}

void UGameplaySettingsMenu::ToggleAutoSlideCamera(bool IsChecked){
	Cast<UGameplaySettingsSave>(Settings)->bAutoSlideCamera = IsChecked;
}

void UGameplaySettingsMenu::UpdateFOVSlider(float Value){
	Cast<UGameplaySettingsSave>(Settings)->CameraFOV = Value;
}

void UGameplaySettingsMenu::UpdateSensitivitySlider(float Value){
	Cast<UGameplaySettingsSave>(Settings)->CameraSensitivity = Value;
}

void UGameplaySettingsMenu::InitializeSettings(){
	const UGameplaySettingsSave* CastedSettings = Cast<UGameplaySettingsSave>(Settings);

	CastedSettings->bInvertCamYAxis ? YAxisCheckBox->SetCheckedState(ECheckBoxState::Checked) : YAxisCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	CastedSettings->bAutoSlideCamera ? AutoSlideCameraCheckBox->SetCheckedState(ECheckBoxState::Checked) : AutoSlideCameraCheckBox->SetCheckedState(ECheckBoxState::Unchecked);

	FOVSlider->SetValue(CastedSettings->CameraFOV);
	SensitivitySlider->SetValue(CastedSettings->CameraSensitivity);

	Super::InitializeSettings();
}

void UGameplaySettingsMenu::UpdateSettings(){
	Super::UpdateSettings();

	if(!IsValid(MainPlayer)){
		return;
	}

	const UGameplaySettingsSave* CastedSettings = Cast<UGameplaySettingsSave>(Settings);

	MainPlayer->GetFollowCamera()->SetFieldOfView(CastedSettings->CameraFOV);
	MainPlayer->SetInvertAxis(CastedSettings->bInvertCamYAxis);
	MainPlayer->SetAutoSlideCamera(CastedSettings->bAutoSlideCamera);
	MainPlayer->SetSensitivity(CastedSettings->CameraSensitivity);
}
