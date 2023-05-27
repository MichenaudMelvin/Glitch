// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/MainAICharacter.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	UWidgetComponent* HealthWidget;

	UEnemyHealthBar* EnemyHealthBarWidget;
};
