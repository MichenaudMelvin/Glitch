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

	ResolutionBox->OnSelectionChanged.AddDynamic(this, &UVideoSettingsMenu::ChangeResolution);
}

void UVideoSettingsMenu::NativePreConstruct(){
	Super::NativePreConstruct();

	if(IsValid(ResolutionBox)){
		ResolutionBox->ClearOptions();
		ResolutionBox->AddOption("800x600");
		ResolutionBox->AddOption("1024x768");
		ResolutionBox->AddOption("1280x720");
		ResolutionBox->AddOption("1280x1024");
		ResolutionBox->AddOption("1366x768");
		ResolutionBox->AddOption("1600x900");
		ResolutionBox->AddOption("1680x1050");
		ResolutionBox->AddOption("1920x1080");

		ResolutionBox->SetSelectedOption("1920x1080");
		ResolutionBox->RefreshOptions();
	}
}

void UVideoSettingsMenu::ToggleVSync(bool IsChecked){
	Cast<UVideoSettingsSave>(Settings)->VSyncEnable = IsChecked;
}

void UVideoSettingsMenu::ChangeResolution(FString SelectedItem, ESelectInfo::Type SelectionType){
	if(SelectionType == ESelectInfo::Direct){
		return;
	}

	FString XString;
	FString YString;
	SelectedItem.Split("x", &XString, &YString);

	const int X = FCString::Atoi(*XString);
	const int Y = FCString::Atoi(*YString);
	Cast<UVideoSettingsSave>(Settings)->Resolution = FIntPoint(X, Y);
}

void UVideoSettingsMenu::InitializeSettings(){

	const UVideoSettingsSave* CastedSettings = Cast<UVideoSettingsSave>(Settings);

	CastedSettings->VSyncEnable ? VSyncCheckBox->SetCheckedState(ECheckBoxState::Checked) : VSyncCheckBox->SetCheckedState(ECheckBoxState::Unchecked);

	const FString Res = FString::FromInt(CastedSettings->Resolution.X) + "x" + FString::FromInt(CastedSettings->Resolution.Y);
	ResolutionBox->SetSelectedOption(Res);

	Super::InitializeSettings();
}

void UVideoSettingsMenu::UpdateSettings() const{
	Super::UpdateSettings();

	Gamemode->UpdateVideoSettings(Cast<UVideoSettingsSave>(Settings));
}
