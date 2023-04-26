// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/Settings/GameplaySettingsMenu.h"
#include "GlitchUEGameMode.h"
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

	FOVSlider->GetSlider()->OnValueChanged.AddDynamic(this, &UGameplaySettingsMenu::UpdateFOVSlider);
	SensibilitySlider->GetSlider()->OnValueChanged.AddDynamic(this, &UGameplaySettingsMenu::UpdateSensibilitySlider);
}

void UGameplaySettingsMenu::NativeDestruct(){
	if(!UGameplayStatics::GetGameMode(GetWorld())->IsA(AGlitchUEGameMode::StaticClass())){
		return;
	}

	Super::NativeDestruct();
}

void UGameplaySettingsMenu::ToggleYAxis(bool IsChecked){
	Cast<UGameplaySettingsSave>(Settings)->bInvertCamYAxis = IsChecked;
}

void UGameplaySettingsMenu::UpdateFOVSlider(float Value){
	Cast<UGameplaySettingsSave>(Settings)->CameraFOV = Value;
}

void UGameplaySettingsMenu::UpdateSensibilitySlider(float Value){
	Cast<UGameplaySettingsSave>(Settings)->CameraSensibility = Value;
}

void UGameplaySettingsMenu::InitializeSettings(){
	const UGameplaySettingsSave* CastedSettings = Cast<UGameplaySettingsSave>(Settings);

	CastedSettings->bInvertCamYAxis ? YAxisCheckBox->SetCheckedState(ECheckBoxState::Checked) : YAxisCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	FOVSlider->SetValue(CastedSettings->CameraFOV);
	SensibilitySlider->SetValue(CastedSettings->CameraSensibility);

	Super::InitializeSettings();
}

void UGameplaySettingsMenu::UpdateSettings() const{

	const UGameplaySettingsSave* CastedSettings = Cast<UGameplaySettingsSave>(Settings);

	MainPlayer->GetFollowCamera()->SetFieldOfView(CastedSettings->CameraFOV);
	MainPlayer->SetInvertAxis(CastedSettings->bInvertCamYAxis);
	MainPlayer->SetSensibility(CastedSettings->CameraSensibility);
}
