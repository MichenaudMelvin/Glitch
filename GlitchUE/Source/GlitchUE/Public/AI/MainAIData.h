// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Assets/PopcornFXEffect.h"
#include "Engine/DataAsset.h"
#include "FMODEvent.h"
#include "MainAIData.generated.h"

UCLASS()
class GLITCHUE_API UMainAIData : public UDataAsset{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Name")
	FString DroneName;

	UPROPERTY(EditDefaultsOnly, Category = "Appearence")
	USkeletalMesh* AIMesh;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UPopcornFXEffect* IdleFX;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UPopcornFXEffect* DeathFX;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Speed = 200;

	UPROPERTY(EditDefaultsOnly, Category =  "Stats")
	float Health = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float AISpawnGlitchValue = 25;

	UPROPERTY(EditDefaultsOnly, Category =  "Stats|Sight")
	UStaticMesh* SightMesh;

	UPROPERTY(EditDefaultsOnly, Category =  "Stats|Sight")
	FVector SightDetectionScale = FVector::OneVector;

	UPROPERTY(EditDefaultsOnly, Category =  "Stats|Behavior")
	float StunTime = 5;

	UPROPERTY(EditDefaultsOnly, Category =  "Stats|Behavior")
	float Damages = 5;

	UPROPERTY(EditDefaultsOnly, Category =  "Stats|Behavior")
	float InvestigatingTime = 1;

	/**
	 * @brief Also used as alerted speed for patrol
	 */
	UPROPERTY(EditDefaultsOnly, Category = "GlitchStats")
	float GlitchSpeed = 500;

	UPROPERTY(EditDefaultsOnly, Category =  "GlitchStats")
	float GlitchHealth = 100;

	UPROPERTY(EditDefaultsOnly, Category =  "GlitchStats|Sight")
	FVector GlitchSightScaleDetection = FVector(3, 3, 3);

	UPROPERTY(EditDefaultsOnly, Category = "GlitchStats")
	float GlitchDuration = 10;

	UPROPERTY(EditDefaultsOnly, Category =  "GlitchStats|Behavior")
	float GlitchDamages = 5;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	UFMODEvent* IdleSound;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	UFMODEvent* DeathSound;
};
