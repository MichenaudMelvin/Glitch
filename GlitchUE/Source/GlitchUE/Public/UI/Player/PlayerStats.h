// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PlayerStats.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UPlayerStats : public UUserWidget{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StatsWidgets", meta = (BindWidget))
	UTextBlock* GoldsText;

public:
	void UpdateDisplayGolds(int Golds) const;
};
