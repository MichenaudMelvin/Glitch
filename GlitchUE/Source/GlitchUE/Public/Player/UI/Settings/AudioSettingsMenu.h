// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/UI/CustomSlider.h"
#include "Player/UI/Settings/SettingsMenu.h"
#include "AudioSettingsMenu.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UAudioSettingsMenu : public USettingsMenu{
	GENERATED_BODY()

public:
	UAudioSettingsMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(EditDefaultsOnly, Category = "SettignsElements", meta = (BindWidget))
	UCustomSlider* MasterVolumeSlider;

	UFUNCTION()
	void UpdateMasterVolumeSlider(float Value);

	UPROPERTY(EditDefaultsOnly, Category = "SettignsElements", meta = (BindWidget))
	UCustomSlider* MusicVolumeSlider;

	UFUNCTION()
	void UpdateMusicVolumeSlider(float Value);

	UPROPERTY(EditDefaultsOnly, Category = "SettignsElements", meta = (BindWidget))
	UCustomSlider* SFXVolumeSlider;

	UFUNCTION()
	void UpdateSFXVolumeSlider(float Value);

	virtual void InitializeSettings() override;

	virtual void UpdateSettings() const override;
};
