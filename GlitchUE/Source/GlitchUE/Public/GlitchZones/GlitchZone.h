// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerZone.h"
#include "GlitchZone.generated.h"

UCLASS()
class GLITCHUE_API AGlitchZone : public APlayerZone{
	GENERATED_BODY()

public:
	AGlitchZone();

protected:
	virtual void BeginPlay() override;

	virtual void OnPlayerEnterZone() override;

	virtual void OnPlayerExitZone() override;

	AAudioManager* AudioManager;

	UPROPERTY(EditDefaultsOnly, Category = "Glitch")
	float GlitchGaugeTick = 0.2;

	UPROPERTY(EditDefaultsOnly, Category = "Glitch")
	float GlitchGaugeValueToAddAtStart = 500;

	UPROPERTY(EditDefaultsOnly, Category = "Glitch")
	float GlitchGaugeValueToAddEveryTick = 100;
};
