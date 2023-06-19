// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "FMODAudioComponent.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "Dissolver.generated.h"

class AMainPlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnPlayerExitDissolver);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnPlayerEnterDissolver);

UCLASS()
class GLITCHUE_API ADissolver : public AActor{
	GENERATED_BODY()

public:
	ADissolver();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Dissolver")
	USphereComponent* DissolveCollider;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Audio")
	UFMODAudioComponent* FMODAudio;

	AGlitchUEGameMode* GameMode;

	bool bPlayerInsideDissolver = false;

	void UpdateRadius();

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	float MaxBorderDistance = 500;

	void BorderSound();

	bool bAllowBorderSound = false;

	UFUNCTION()
	void OnSwitchPhase(const EPhases NewPhase);

	AMainPlayer* MainPlayer;

public:
	/**
	 * @brief Should be use at begin play or in editor only
	 */
	void UpdateShaderFX();

	UFUNCTION(BlueprintCallable)
	void DissolveTo(const float TargetRadius);

	float GetRadius() const;

	UFUNCTION(BlueprintCallable)
	void ForceDissolverValue(const float TargetRadius);

	UFUNCTION(BlueprintCallable)
	void ResetDissolveDuration();

protected:
	void PlayFX();

	UFUNCTION(CallInEditor, Category = "Editor")
	void SetEnvironnementToMed();

	UFUNCTION(CallInEditor, Category = "Editor")
	void SetEnvironnementToTech();

	UFUNCTION()
	void LerpRadius(float Value);

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, Category = "Radius")
	float Radius = 2500;

	UPROPERTY(EditAnywhere, Category = "Radius")
	float MaxRadius = 5000;

	UPROPERTY(EditDefaultsOnly, Category = "Outline")
	float RadiusScale;

	UPROPERTY(EditDefaultsOnly, Category = "Outline")
	FLinearColor Emissive = FColor(1, 1, 1, 0);

	UPROPERTY(EditDefaultsOnly, Category = "Outline")
	float SecondTilling;

	UPROPERTY(EditDefaultsOnly, Category = "Outline")
	float Power;

	UPROPERTY(EditDefaultsOnly, Category = "Outline")
	float TexRez;

	UPROPERTY(EditAnywhere, Category = "Time")
	float FXDuration = 1;

	float CurrentRadiusValue;

	float TargetRadiusValue;

	FTimeline DissolveTimeline;

	UCurveFloat* ZeroToOneCurve;

	UPROPERTY(EditDefaultsOnly)
	UMaterialParameterCollection* MPCDissolver;

public:
	float GetMaxRadius() const;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates")
	FKOnPlayerExitDissolver OnPlayerExitDissolver;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates")
	FKOnPlayerEnterDissolver OnPlayerEnterDissolver;
};
