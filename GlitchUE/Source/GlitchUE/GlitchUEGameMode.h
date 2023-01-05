// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GlitchUEGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnGlitchMax);

UENUM(BlueprintType)
enum class EPhases : uint8 {

	Infiltration
	UMETA(DisplayName = "Infiltration"),

	TowerDefense
	UMETA(DisplayName = "TowerDefense"),
};

UENUM(BlueprintType)
enum class ELevelState : uint8 {

	Normal
	UMETA(DisplayName = "Normal"),

	Alerted
	UMETA(DisplayName = "Alerted"),
};

UCLASS(minimalapi)
class AGlitchUEGameMode : public AGameModeBase{
	GENERATED_BODY()

public:
	AGlitchUEGameMode();

protected:
	virtual void BeginPlay() override;

	EPhases CurrentPhase = EPhases::Infiltration;

	ELevelState LevelState = ELevelState::Normal;

	float GlitchValue;

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
	void SetLevelState(ELevelState newState);

	UFUNCTION(BlueprintCallable, Exec, Category = "Glitch")
	void AddGlitch(float AddedValue);
};
