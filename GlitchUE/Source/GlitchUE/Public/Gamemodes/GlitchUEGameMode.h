// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Engine/SceneCapture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Gamemodes/MainGamemode.h"
#include "Helpers/UsefulStructs.h"
#include "Saves/SaveInterface.h"
#include "UI/Gameplay/CreditsScreen.h"
#include "GlitchUEGameMode.generated.h"

class AAudioManager;
class AMainPlayer;
class AMainPlayerController;
class UPlayerStats;
class ANexus;
class AWaveManager;
class UWorldSave;
class ADissolver;

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnStartStealthTimer);

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

	UPROPERTY()
	AMainPlayer* MainPlayer;

	UPROPERTY()
	AMainPlayerController* MainPlayerController;

	UPROPERTY()
	UPlayerStats* PlayerStatsWidget;

	EPhases CurrentPhase = EPhases::Infiltration;

	ELevelState LevelState = ELevelState::Normal;

	AWaveManager* WaveManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Objectives")
	bool bUseAutoObjectivesForPlayer = true;

	UPROPERTY(EditDefaultsOnly, Category = "Objectives")
	FString ObjectiveEndMessage = "You unlock all generators in the level";

	/**
	 * @brief show up when you can interact with the nexus
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Objectives")
	FString AdditionalStealthEndMessage = "You can now interact with the nexus";

	void UpdatePlayerObjectives() const;

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|World")
	FKOnSwitchPhases OnSwitchPhases;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|World")
	FKOnSwitchLevelState OnSwitchLevelState;

	UFUNCTION(BlueprintCallable)
	void GlobalWorldSave(const int Index);

	UFUNCTION(BlueprintCallable, Category = "Save")
	virtual void GlobalWorldLoad(const int Index) override;

	/**
	 * @brief if TimerValue is equal to 0 it will uses the stealth timer variable
	 * @param TimerValue else this variable will be used for the timer
	 */
	UFUNCTION(BlueprintCallable, Exec, Category = "Timer")
	virtual void LaunchStealthTimer(float TimerValue = 0);

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates")
	FKOnStartStealthTimer OnStartStealthTimer;

	float GetStealthTimer() const;

	float GetGoldTimerMultiplier() const;

	bool CanStartTowerDefense() const;

	UFUNCTION(Exec)
	void ForceEndStealthPhase() const;

	void UpdateActivatedCatalyseurAmount(const bool Increase = true);

	int GetActivatedCatalyseurNum() const;

	bool UseAutoObjectivesForPlayer() const;

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

	UPROPERTY(BlueprintReadOnly)
	int CatalyseurNumberInLevel = 0;

	ANexus* Nexus;

	ADissolver* Dissolver;

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

public:
	UFUNCTION(BlueprintCallable)
	void RemoveStealthTime(const float RemovedTime) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ending")
	float TransitionDuration = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Ending")
	TSubclassOf<UCreditsScreen> CreditsScreenWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Ending|Keyboard")
	TArray<FTchatStruct> KeyboardEndingMessages;

	UPROPERTY(EditDefaultsOnly, Category = "Ending|Nexus")
	float EndDissolveDuration = 2.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Ending|Keyboard")
	FString EndKeyboardMessage = "The game has been fully corrupted, all the saved games from all of the folders have been corrupted. Nobody will play it again. Congratulations, but now all of these players will be sad, was it the right choice ?";

	UPROPERTY(EditDefaultsOnly, Category = "Ending|Nexus")
	FString EndNexusMessage = "All the players will see their favourite game again. But is it truly good for them ? Was it the right choice ?";

public:
	UFUNCTION(BlueprintCallable, Exec, Category = "Ending|Keyboard")
	void CallKeyboardEnding();

	UFUNCTION()
	void KeyboardMessagesEnd();

	UFUNCTION(BlueprintCallable, Exec, Category = "Ending|Nexus")
	void CallNexusEnding();

protected:
	UFUNCTION(Exec, Category = "Glitch")
	void GlitchUpgradeAlliesUnits() const;

	UFUNCTION(Exec, Category = "Glitch")
	void GlitchUpgradeEnemiesAI() const;

	UFUNCTION(Exec, Category = "Glitch")
	void GlitchUpgradePlayer() const;

	UFUNCTION(Exec, Category = "Glitch")
	void GlitchUpgradeWorld() const;

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
