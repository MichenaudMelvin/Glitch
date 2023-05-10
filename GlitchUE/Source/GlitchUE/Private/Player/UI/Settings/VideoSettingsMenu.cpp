// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/Settings/VideoSettingsMenu.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Saves/Settings/VideoSettingsSave.h"

UVideoSettingsMenu::UVideoSettingsMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
	SettingClass = UVideoSettingsSave::StaticClass();

	SettingCategory = FText::FromString("Video");
}

void UVideoSettingsMenu::NativeOnInitialized(){
	Super::NativeOnInitialized();

	VSyncCheckBox->OnCheckStateChanged.AddDynamic(this, &UVideoSettingsMenu::ToggleVSync);

	ResolutionBox->OnSelectionChanged.AddDynamic(this, &UVideoSettingsMenu::ChangeResolution);

	WindowModeBox->OnSelectionChanged.AddDynamic(this, &UVideoSettingsMenu::ChangeWindowMode);
}

void UVideoSettingsMenu::NativePreConstruct(){
	Super::NativePreConstruct();

	if(IsValid(ResolutionBox) && IsValid(GameUserSettings)){
		TArray<FIntPoint> Array;

		switch (GameUserSettings->GetFullscreenMode()) {
			case EWindowMode::Fullscreen:
				UKismetSystemLibrary::GetSupportedFullscreenResolutions(Array);
				break;
			case EWindowMode::WindowedFullscreen:
				UKismetSystemLibrary::GetSupportedFullscreenResolutions(Array);
				break;
			case EWindowMode::Windowed:
				UKismetSystemLibrary::GetSupportedFullscreenResolutions(Array);
				break;
			case EWindowMode::NumWindowModes:
				UKismetSystemLibrary::GetSupportedFullscreenResolutions(Array);
				break;
		}

		ResolutionBox->ClearOptions();
		for(int i = 0; i < Array.Num(); i++){
			ResolutionBox->AddOption(FString::FromInt(Array[i].X) + "x" + FString::FromInt(Array[i].Y));
		}

		const FIntPoint CurrentResolution = GameUserSettings->GetScreenResolution();

		auto a = FString::FromInt(CurrentResolution.X) + "x" + FString::FromInt(CurrentResolution.Y);
		UE_LOG(LogTemp, Warning, TEXT("The vector value is: %s"), *a);

		ResolutionBox->SetSelectedOption(FString::FromInt(CurrentResolution.X) + "x" + FString::FromInt(CurrentResolution.Y));
	}

	if(IsValid(WindowModeBox)){
		WindowModeBox->ClearOptions();

		WindowModeBox->AddOption(LexToString(EWindowMode::Fullscreen));
		WindowModeBox->AddOption(LexToString(EWindowMode::WindowedFullscreen));
		WindowModeBox->AddOption(LexToString(EWindowMode::Windowed));
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

void UVideoSettingsMenu::ChangeWindowMode(FString SelectedItem, ESelectInfo::Type SelectionType){
	if(SelectionType == ESelectInfo::Direct){
		return;
	}

	UVideoSettingsSave* CastedSettings = Cast<UVideoSettingsSave>(Settings);

	CastedSettings->WindowMode = EWindowMode::ConvertIntToWindowMode(WindowModeBox->GetSelectedIndex());
	GameUserSettings->SetFullscreenMode(CastedSettings->WindowMode);
}

void UVideoSettingsMenu::InitializeSettings(){

	const UVideoSettingsSave* CastedSettings = Cast<UVideoSettingsSave>(Settings);

	CastedSettings->VSyncEnable ? VSyncCheckBox->SetCheckedState(ECheckBoxState::Checked) : VSyncCheckBox->SetCheckedState(ECheckBoxState::Unchecked);

	const FString Res = FString::FromInt(CastedSettings->Resolution.X) + "x" + FString::FromInt(CastedSettings->Resolution.Y);
	ResolutionBox->SetSelectedOption(Res);

	WindowModeBox->SetSelectedOption(LexToString(CastedSettings->WindowMode));

	Super::InitializeSettings();
}

void UVideoSettingsMenu::UpdateSettings() const{
	Super::UpdateSettings();

	const UVideoSettingsSave* CastedSettings = Cast<UVideoSettingsSave>(Settings);

	Gamemode->UpdateVideoSettings(CastedSettings);
	GameUserSettings->SetFullscreenMode(CastedSettings->WindowMode);
}
