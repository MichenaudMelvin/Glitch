// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "EnemyHealthBar.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UEnemyHealthBar : public UUserWidget{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "HealthBar", meta = (BindWidget))
	UProgressBar* HealthBar;

public:
	void UpdateHealth(const float HealthPercentage) const;
};
