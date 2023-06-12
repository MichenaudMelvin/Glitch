// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/MainAICharacter.h"
#include "Components/BillboardWidgetComponent.h"
#include "UI/Gameplay/EnemyHealthBar.h"
#include "FocusAICharacter.generated.h"

UCLASS(Abstract)
class GLITCHUE_API AFocusAICharacter : public AMainAICharacter{
	GENERATED_BODY()

public:
	AFocusAICharacter();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateWidgetHealth();

	virtual void SetCurrentData(UMainAIData* NewData) override;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	UBillboardWidgetComponent* HealthWidget;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UPopcornFXEmitterComponent* AttackFX;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UPopcornFXEmitterComponent* HitFX;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UFMODAudioComponent* AttackAudioComp;

	UPROPERTY()
	UEnemyHealthBar* EnemyHealthBarWidget;

	UFUNCTION()
	void TakeDamages();

public:
	UPopcornFXEmitterComponent* GetAttackFX() const;

	UFMODAudioComponent* GetAttackAudioComp() const;
};
