// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/MainGamemode.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Saves/Settings/AudioSettingsSave.h"
#include "FMODBlueprintStatics.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/Settings/VideoSettingsSave.h"

AMainGamemode::AMainGamemode(){
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

void AMainGamemode::BeginPlay(){
	Super::BeginPlay();

	GameUserSettings = UGameUserSettings::GetGameUserSettings();
	UpdateGlobalSettings();
}

void AMainGamemode::UpdateGlobalSettings() const{
	const UAudioSettingsSave* AudioSettings = Cast<UAudioSettingsSave>(UUsefullFunctions::LoadSave(UAudioSettingsSave::StaticClass(), 0));

	UFMODBlueprintStatics::VCASetVolume(MasterBank, AudioSettings->MasterVolume);
	UFMODBlueprintStatics::VCASetVolume(MusicBank, AudioSettings->MusicVolume);
	UFMODBlueprintStatics::VCASetVolume(SFXBank, AudioSettings->SFXVolume);

	const UVideoSettingsSave* VideoSettings = Cast<UVideoSettingsSave>(UUsefullFunctions::LoadSave(UVideoSettingsSave::StaticClass(), 0));

	GameUserSettings->SetVSyncEnabled(VideoSettings->VSyncEnable);
	GameUserSettings->SetScreenResolution(VideoSettings->Resolution);
	GameUserSettings->ApplySettings(false);
}

void AMainGamemode::SetGlobalTimeDilation(float TimeDilation) const{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
}

void AMainGamemode::CrashGame() const{
	UE_LOG(LogTemp, Fatal, TEXT("CRASH"));
}

void AMainGamemode::CollectGarbage() const{
	UKismetSystemLibrary::CollectGarbage();
}

void AMainGamemode::RemoveAllWidgets() const{
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
}
