// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FMODVCA.h"
#include "Engine/ExponentialHeightFog.h"
#include "Saves/Settings/AudioSettingsSave.h"
#include "Saves/Settings/VideoSettingsSave.h"
#include "MainGamemode.generated.h"

UCLASS(Abstract)
class GLITCHUE_API AMainGamemode : public AGameModeBase{
	GENERATED_BODY()

public:
	AMainGamemode();

protected:
	virtual void BeginPlay() override;

public:
	void UpdateGlobalSettings() const;

	void UpdateVideoSettings(const UVideoSettingsSave* VideoSettings) const;

	void UpdateAudioSettings(const UAudioSettingsSave* AudioSettings) const;

protected:
	UPROPERTY()
	UGameUserSettings* GameUserSettings;

	UPROPERTY()
	AExponentialHeightFog* ExponentialFog;

	UPROPERTY()
	UFMODVCA* MasterBank;

	UPROPERTY()
	UFMODVCA* MusicBank;

	UPROPERTY()
	UFMODVCA* SFXBank;

#pragma region ConsoleCommands

private:
	UFUNCTION(Exec)
	void SetGlobalTimeDilation(float TimeDilation) const;

	UFUNCTION(Exec)
	void CrashGame() const;

	UFUNCTION(Exec)
	void CollectGarbage() const;

	// this one should be used for showing the game without UI, it can easily causes glitch otherwise
	// you can use the BindNormalMode command to fix glitch
	UFUNCTION(Exec)
	void RemoveAllWidgets() const;

#pragma endregion
};
