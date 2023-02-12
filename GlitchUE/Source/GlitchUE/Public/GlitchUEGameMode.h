// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AI/Waves/WaveManager.h"
#include "Player/MainPlayer.h"
#include "Curves/CurveLinearColor.h"
#include "Saves/AbstractSave.h"
#include "Saves/WorldSave.h"
#include "GlitchUEGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnGlitchMax);

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
		RandomFX,
	};
}

UENUM(BlueprintType)
enum class ELevelOptions : uint8{

	WithoutSave,

	FastLoad,
};

UCLASS(minimalapi)
class AGlitchUEGameMode : public AGameModeBase{
	GENERATED_BODY()

public:
	AGlitchUEGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "Glitch")
	AMainPlayer* MainPlayer;

	EPhases CurrentPhase = EPhases::Infiltration;

	ELevelState LevelState = ELevelState::Normal;

	AWaveManager* WaveManager;

	UPROPERTY(BlueprintReadWrite, Category = "Saves")
	UWorldSave* WorldSave;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Save(UAbstractSave* SaveObject);
	void Save_Implementation(UAbstractSave* SaveObject);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UAbstractSave* Load(TSubclassOf<UAbstractSave> SaveClass, int UserIndex);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FastSave();
	void FastSave_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FastLoad();
	void FastLoad_Implementation();
	
	float GlitchValue;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Glitch")
	float GlitchMaxValue;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates")
	FKOnGlitchMax OnGlitchMax;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Phases")
	EPhases GetPhases();

	UFUNCTION(BlueprintCallable, Exec, Category = "Phases")
	void SetNewPhase(EPhases NewPhase);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LevelState")
	ELevelState GetLevelState();

	UFUNCTION(BlueprintCallable, Exec, Category = "LevelState")
	void SetLevelState(ELevelState NewState);

	UFUNCTION(BlueprintCallable, Exec, Category = "Glitch")
	void AddGlitch(float AddedValue);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Glitch")
	float GetCurrentGlitchValue();

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
	void GlitchUpgradeAlliesUnits();

	UFUNCTION(Exec, Category = "Glitch")
	void GlitchUpgradeEnemiesAI();

	UFUNCTION(Exec, Category = "Glitch")
	void GlitchUpgradePlayer();

	UFUNCTION(Exec, Category = "Glitch")
	void GlitchRandomFX();

	void CheckAvailableGlitchEvents();

#pragma region ConsoleCommands

private:
	UFUNCTION(Exec)
	void SetGlobalTimeDilation(float TimeDilation);

	UFUNCTION(Exec)
	void NextWave();

	UFUNCTION(Exec)
	void GoToWave(int NewWave);
	
	UFUNCTION(Exec)
	void CrashGame();

	UFUNCTION(Exec)
	void ToggleSpectatorMode();
		
#pragma endregion

};
