// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/MainPlayer.h"
#include "Engine/SceneCapture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Gamemodes/MainGamemode.h"
#include "Saves/SaveInterface.h"
#include "GlitchUEGameMode.generated.h"

class AAudioManager;

UENUM(BlueprintType)
enum class EPhases : uint8 {

	Infiltration,

	TowerDefense,
};

UENUM(BlueprintType)
enum class ELevelState : uint8 {

	Normal,

	Alerted,
};

namespace Glitch {
	UENUM(BlueprintType)
	enum EGlitchEvents : uint8{
		UpgradeWorld,
		UpgradeEnemiesAI,
		UpgradePlayer,
		UpgradeAlliesUnits,
	};

	static int StealthIndex = 1;

	static int TowerDefenseIndex = 3;

	static int BothIndex = TowerDefenseIndex - StealthIndex;
}

UENUM(BlueprintType)
enum class ELevelOptions : uint8{

	WithoutSave,

	FastLoad,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnGlitchMax);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnSwitchPhases, EPhases, NewPhases);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnSwitchLevelState, ELevelState, NewState);

UCLASS(minimalapi)
class AGlitchUEGameMode : public AMainGamemode, public ISaveInterface{
	GENERATED_BODY()

public:
	AGlitchUEGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	void InitializeWorld();

	void InitializeWorldSave(TArray<FString> LevelSettings);

	UPROPERTY(BlueprintReadOnly, Category = "Glitch")
	AMainPlayer* MainPlayer;

	EPhases CurrentPhase = EPhases::Infiltration;

	ELevelState LevelState = ELevelState::Normal;

	AWaveManager* WaveManager;

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|World")
	FKOnSwitchPhases OnSwitchPhases;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|World")
	FKOnSwitchLevelState OnSwitchLevelState;

	UFUNCTION(BlueprintCallable)
	void GlobalWorldSave(const int Index);

	UFUNCTION(BlueprintCallable)
	virtual void GlobalWorldLoad(const int Index) override;

	/**
	 * @brief 
	 * @param TimerValue if TimerValue is equal to 0 it will uses the stealth timer variable
	 */
	UFUNCTION(Exec)
	virtual void LaunchStealthTimer(float TimerValue = 0);

	bool CanStartTowerDefense() const;

	UFUNCTION(Exec)
	void ForceEndStealthPhase() const;

	void UpdateActivatedCatalyseurAmount(const bool Increase = true);

	int GetActivatedCatalyseurNum() const;

protected:
	UWorldSave* StealthWorldSave(UWorldSave* CurrentSave);

	UWorldSave* TowerDefenseWorldSave(UWorldSave* CurrentSave);

	UWorldSave* StealthWorldLoad(UWorldSave* CurrentSave);

	UWorldSave* TowerDefenseWorldLoad(UWorldSave* CurrentSave);

	UFUNCTION()
	void EndStealthTimer();

	// Timer in seconds
	UPROPERTY(EditDefaultsOnly, Category = "Stealth")
	float StealthTimer = 120.0f;

	/**
	 * @brief This variable will be multiplied with the remaining stealth time when the player activate the nexus. eg : timer is at 20sec : golds added to the player will be 20 * GoldTimerMultiplier
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Stealth")
	float GoldTimerMultiplier = 1.5f;

	// The require amount of catalyseur to active to start tower defense
	UPROPERTY(EditDefaultsOnly, Category = "Tower Defense")
	int MaxCatalyseurToActivate = 2;

	UPROPERTY(BlueprintReadOnly)
	int CurrentActivatedCatalyseurs = 0;

	ASceneCapture2D* SceneCapture;

	ANexus* Nexus;

	ADissolver* Dissolver;

	UPROPERTY(EditDefaultsOnly)
	TArray<UTextureRenderTarget2D*> SaveRenderTarget;

	UPROPERTY(EditDefaultsOnly)
	TArray<UMaterial*> SaveMaterials;

	UPROPERTY(EditDefaultsOnly)
	int MaxLoadSaveTime = 3;

	float GlitchValue;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Glitch")
	float GlitchMaxValue = 5000;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates")
	FKOnGlitchMax OnGlitchMax;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Phases")
	EPhases GetPhases() const;

	UFUNCTION(BlueprintCallable, Exec, Category = "Phases")
	void SetNewPhase(const EPhases NewPhase);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LevelState")
	ELevelState GetLevelState() const;

	UFUNCTION(BlueprintCallable, Exec, Category = "LevelState")
	void SetLevelState(const ELevelState NewState);

	UFUNCTION(BlueprintCallable, Exec, Category = "Glitch")
	void AddGlitch(const float AddedValue);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Glitch")
	float GetCurrentGlitchValue() const;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UMaterialParameterCollection* AlertedMaterial;

private:
	FTimeline LevelStateTimeline;

	UCurveLinearColor* ColorCurve;

	bool RequestNormalState = false;

	ETimelineDirection::Type LevelStateTimelineDirection;

	ETimelineDirection::Type BlinkingTimelineDirection;

	UFUNCTION()
	void UpdateLevelColor(FLinearColor NewColor);

	UFUNCTION()
	void AlertLevelFinished();

	FTimeline BlinkingTimeline;

	UCurveLinearColor* BlinkingCurve;

	UFUNCTION()
	void BlinkingFinished();

protected:
	UFUNCTION(Exec, Category = "Glitch")
	void GlitchUpgradeAlliesUnits() const;

	UFUNCTION(Exec, Category = "Glitch")
	void GlitchUpgradeEnemiesAI() const;

	UFUNCTION(Exec, Category = "Glitch")
	void GlitchUpgradePlayer() const;

	UFUNCTION(Exec, Category = "Glitch")
	void GlitchUpgradeWorld() const;

	void CheckAvailableGlitchEvents() const;

	UPROPERTY(EditDefaultsOnly, Category = "Glitch")
	int NumberOfEnemiesToAffect = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Glitch")
	int NumberOfAlliesUnitsToAffect = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Glitch")
	float GlitchReduceStealthTimer = 10;

#pragma region ConsoleCommands

private:
	UFUNCTION(Exec)
	void SetSelfTimeDilation(float TimeDilation) const;

	UFUNCTION(Exec)
	void NextWave() const;

	UFUNCTION(Exec)
	void GoToWave(const int NewWave) const;

	UFUNCTION(Exec)
	void ToggleSpectatorMode(const bool bToggleAtLocation = false) const;

	UFUNCTION(Exec)
	void Dissolve(const float Value) const;

#pragma endregion

};
