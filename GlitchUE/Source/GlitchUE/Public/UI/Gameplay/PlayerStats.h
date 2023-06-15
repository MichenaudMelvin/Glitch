// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "PlayerStats.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UPlayerStats : public UUserWidget{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StatsWidgets", meta = (BindWidget))
	UTextBlock* GoldsText;

	UPROPERTY(EditDefaultsOnly, Category = "StatsWidgets", meta = (BindWidget))
	UTextBlock* NexusHealth;

	UPROPERTY(EditDefaultsOnly, Category = "StatsWidgets", meta = (BindWidget))
	UProgressBar* NexusHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "StatsWidgets", meta = (BindWidget))
	UTextBlock* ObjectiveText;

	UPROPERTY(EditDefaultsOnly, Category = "StatsWidgets", meta = (BindWidget))
	UTextBlock* AdditionalText;

public:
	UFUNCTION()
	void UpdateDisplayGolds(int Golds);

	void UpdateNexusHealth(const float NewHealth, const float NexusMaxHealth)const;

	UFUNCTION(BlueprintCallable)
	void UpdateObjectivesText(const FString NewObjective) const;

	UFUNCTION(BlueprintCallable)
	void UpdateAdditionalText(const FString NewMessage) const;
};
