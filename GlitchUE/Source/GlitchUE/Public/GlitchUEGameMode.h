// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/MainPlayer.h"
#include "Engine/SceneCapture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Saves/SaveInterface.h"
#include "GlitchUEGameMode.generated.h"

class UWorldSave;
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

UENUM(BlueprintType)
namespace EGlitchEvent {
	enum Type{
		UpgradeAlliesUnits,
		UpgradeEnemiesAI,
		UpgradePlayer,
	};
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
class AGlitchUEGameMode : public AGameModeBase, public ISaveInterface{
	GENERATED_BODY()

public:
	AGlitchUEGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	void InitializeWorld();

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

protected:
	ASceneCapture2D* SceneCapture;

	UPROPERTY(EditDefaultsOnly)
	TArray<UTextureRenderTarget2D*> SaveRenderTarget;

	UPROPERTY(EditDefaultsOnly)
	TArray<UMaterial*> SaveMaterials;

	UPROPERTY(EditDefaultsOnly)
	int MaxLoadSaveTime = 3;

	float GlitchValue;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Glitch")
	float GlitchMaxValue;

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

	void CheckAvailableGlitchEvents() const;

	UPROPERTY(EditDefaultsOnly, Category = "Glitch")
	int NumberOfEnemiesToAffect = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Glitch")
	int NumberOfAlliesUnitsToAffect = 5;

#pragma region ConsoleCommands

private:
	UFUNCTION(Exec)
	void SetGlobalTimeDilation(float TimeDilation) const;

	UFUNCTION(Exec)
	void SetSelfTimeDilation(float TimeDilation) const;

	UFUNCTION(Exec)
	void NextWave() const;

	UFUNCTION(Exec)
	void GoToWave(const int NewWave) const;

	UFUNCTION(Exec)
	void CrashGame() const;

	UFUNCTION(Exec)
	void ToggleSpectatorMode(const bool bToggleAtLocation = false) const;

	UFUNCTION(Exec)
	void Dissolve(const float Value) const;

	UFUNCTION(Exec)
	void CollectGarbage() const;

	// this one should be used for showing the game without UI, it can easily causes glitch otherwise
	// you can use the BindNormalMode command to fix glitch
	UFUNCTION(Exec)
	void RemoveAllWidgets() const;

#pragma endregion

};
