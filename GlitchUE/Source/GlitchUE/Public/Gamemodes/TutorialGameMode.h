// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlitchUEGameMode.h"
#include "TutorialGameMode.generated.h"

UCLASS(Abstract)
class GLITCHUE_API ATutorialGameMode : public AGlitchUEGameMode{
	GENERATED_BODY()

protected:
	virtual void LaunchStealthTimer(float TimerValue) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stealth")
	bool CanLaunchStealthTimer = false;
};
