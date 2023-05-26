// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "PlayerZone.h"
#include "CatalyseurZone.generated.h"

UCLASS()
class GLITCHUE_API ACatalyseurZone : public APlayerZone{
	GENERATED_BODY()

public:
	ACatalyseurZone();

protected:
	virtual void BeginPlay() override;

	virtual void OnPlayerEnterZone() override;

	virtual void OnPlayerExitZone() override;

	/**
	 * @brief Timer in seconds 
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Catalyseur Zone")
	float ResetLevelStateDuration = 1;

	UFUNCTION()
	void OnSwitchPhases(EPhases NewPhase);
};
