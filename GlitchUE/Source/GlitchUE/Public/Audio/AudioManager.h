// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "FMODEvent.h"
#include "FMODAudioComponent.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "FX/Dissolver.h"
#include "AudioManager.generated.h"

UCLASS(Abstract)
class GLITCHUE_API AAudioManager : public AActor{
	GENERATED_BODY()

public:
	AAudioManager();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	UFMODAudioComponent* FMODAudioComp;

	UCurveFloat* ZeroToOneCurve;

	UCurveFloat* FadeInAndOutCurve;

	AGlitchUEGameMode* GameMode;

	UPROPERTY()
	ADissolver* Dissolver;

	UPROPERTY()
	ADissolver* bPlayerInsideDissolver;

	UPROPERTY()
	AMainPlayer* Player;

	FTimeline VolumeTimeline;

	FTimeline FadeInAndOutTimeline;

	FTimeline ParameterTimeline;

	FName CurrentParameter;

	UFUNCTION()
	void FadeVolume(float Alpha);

	UFUNCTION()
	void FadeParameterValue(float Alpha);

	// used when switching to stealth to tower defense
	UPROPERTY(EditDefaultsOnly, Category = "Phases")
	float FadePhaseTransition = 1;

	UPROPERTY(EditAnywhere, Category = "Music")
	UFMODEvent* StartMusic;

	UPROPERTY()
	UFMODEvent* StealthMusic;

	UPROPERTY()
	UFMODEvent* StealthAlarm;

	UPROPERTY()
	UFMODEvent* TowerDefenseMusic;

	UPROPERTY()
	UFMODEvent* PauseMusic;

	UPROPERTY()
	UFMODEvent* TargetMusic;

	UFUNCTION()
	void SetTowerDefenseMusic();

	UFUNCTION()
	void SetStealthMusic();

	UFUNCTION()
	void SetStealthAudio(const ELevelState LevelState);

	UFUNCTION()
	void SetPauseMusic();

	TArray<int> TowerDefenseLayerValues = { 0, 1, 2 };

	UFUNCTION()
	void OnPlayerEnterDissolver();

	UFUNCTION()
	void OnPlayerExitDissolver();

public:
	UFUNCTION(BlueprintCallable, Category = "Audio|Fade")
	void FadeParameter(const FName Param, const bool bFadeIn = true);

	UFUNCTION(BlueprintCallable, Category = "Audio|Parameter")
	void SetParameter(const FName Param, const float Value) const;

	UFUNCTION(BlueprintCallable, Category = "Audio|Fade")
	void FadeInMusic(const FOnTimelineEvent FinishEvent, const float FadeDuration);

	UFUNCTION(BlueprintCallable, Category = "Audio|Fade")
	void FadeOutMusic(const FOnTimelineEvent FinishEvent, const float FadeDuration);

	UFUNCTION(BlueprintCallable, Category = "Audio|Fade")
	void FadeToMusic(UFMODEvent* NewMusic, const float FadeDuration);

	void SwitchToTowerDefenseMusic();

	void SwitchToPauseMusic();

	void UpdateTowerDefenseMusic();

	UFMODAudioComponent* GetAudioComp() const;

	float GetFadeDuration() const;
};
