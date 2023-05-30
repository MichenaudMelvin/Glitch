// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PlayerStats.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UPlayerStats : public UUserWidget{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "StatsWidgets", meta = (BindWidget))
	UTextBlock* GoldsText;

	UPROPERTY(EditDefaultsOnly, Category = "StatsWidgets", meta = (BindWidget))
	UTextBlock* WaveText;

	UPROPERTY(EditDefaultsOnly, Category = "StatsWidgets", meta = (BindWidget))
	UTextBlock* NexusHealth;

	UPROPERTY(EditDefaultsOnly, Category = "StatsWidgets", meta = (BindWidget))
	UTextBlock* ObjectiveText;

	UPROPERTY(EditDefaultsOnly, Category = "StatsWidgets", meta = (BindWidget))
	UTextBlock* TutorialText;

	UFUNCTION()
	void SwitchPhases(EPhases NewPhase);

public:
	void UpdateDisplayGolds(int Golds) const;

	UFUNCTION()
	void UpdateWaveNumber(int CurrentWave);

	void UpdateNexusHealth(const float NewHealth)const;

	UFUNCTION(BlueprintCallable)
	void UpdateObjectivesText(const FString NewObjective) const;

	UFUNCTION(BlueprintCallable)
	void UpdateTutorialText(const FString NewTutorial) const;
};
