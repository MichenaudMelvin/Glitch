// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "FMODEvent.h"
#include "FMODAudioComponent.h"
#include "GlitchUEGameMode.h"
#include "AudioManager.generated.h"

UCLASS(Abstract)
class GLITCHUE_API AAudioManager : public AActor{
	GENERATED_BODY()

public:
	AAudioManager();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	UFMODAudioComponent* FMODAudioComp;

	UCurveFloat* ZeroToOneCurve;

	AGlitchUEGameMode* GameMode;

	FTimeline VolumeTimeline;

	FTimeline ParameterTimeline;

	FName CurrentParameter;

	UFUNCTION()
	void FadeVolume(float Alpha);

	UFUNCTION()
	void FadeParameterValue(float Alpha);

	// used when switching to stealth to tower defense
	UPROPERTY(EditDefaultsOnly, Category = "Phases")
	float FadePhaseTransition = 1;

	UPROPERTY()
	UFMODEvent* StealthMusic;

	UPROPERTY()
	UFMODEvent* TowerDefenseMusic;

	UFUNCTION()
	void SetTowerDefenseMusic();

	UFUNCTION()
	void SetStealthAudio(const ELevelState LevelState);

	float TowerDefenseLayer = 0;

public:
	UFUNCTION(BlueprintCallable, Category = "Audio|Fade")
	void FadeParameter(const FName Param, const bool bFadeIn = true);

	UFUNCTION(BlueprintCallable, Category = "Audio|Parameter")
	void SetParameter(const FName Param, const float Value) const;

	UFUNCTION(BlueprintCallable, Category = "Audio|Fade")
	void FadeInMusic(const FOnTimelineEvent FinishEvent, const float FadeDuration);

	UFUNCTION(BlueprintCallable, Category = "Audio|Fade")
	void FadeOutMusic(const FOnTimelineEvent FinishEvent, const float FadeDuration);

	void SwitchToTowerDefenseMusic();

	void UpdateTowerDefenseMusic();

	UFMODAudioComponent* GetAudioComp() const;
};
