// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/MainGamemode.h"
#include "GameFramework/GameUserSettings.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "FMODBlueprintStatics.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Helpers/FunctionsLibrary/UsefulFunctions.h"
#include "Kismet/GameplayStatics.h"

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

	ExponentialFog = Cast<AExponentialHeightFog>(UGameplayStatics::GetActorOfClass(GetWorld(), AExponentialHeightFog::StaticClass()));

	GameUserSettings = UGameUserSettings::GetGameUserSettings();
	UpdateGlobalSettings();
}

void AMainGamemode::UpdateGlobalSettings() const{
	const UVideoSettingsSave* VideoSettings = Cast<UVideoSettingsSave>(UUsefulFunctions::LoadSave(UVideoSettingsSave::StaticClass(), 0));
	UpdateVideoSettings(VideoSettings);

	const UAudioSettingsSave* AudioSettings = Cast<UAudioSettingsSave>(UUsefulFunctions::LoadSave(UAudioSettingsSave::StaticClass(), 0));
	UpdateAudioSettings(AudioSettings);
}

void AMainGamemode::UpdateVideoSettings(const UVideoSettingsSave* VideoSettings) const{
	GameUserSettings->SetVSyncEnabled(VideoSettings->bVSyncEnable);
	GameUserSettings->SetScreenResolution(VideoSettings->Resolution);
	GameUserSettings->ApplySettings(false);

	if(IsValid(ExponentialFog)){
		ExponentialFog->GetComponent()->SetVisibility(VideoSettings->bVolumetricLighting);
	}
}

void AMainGamemode::UpdateAudioSettings(const UAudioSettingsSave* AudioSettings) const{
	UFMODBlueprintStatics::VCASetVolume(MasterBank, AudioSettings->MasterVolume);
	UFMODBlueprintStatics::VCASetVolume(MusicBank, AudioSettings->MusicVolume);
	UFMODBlueprintStatics::VCASetVolume(SFXBank, AudioSettings->SFXVolume);
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
