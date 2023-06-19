// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConditionScreen.h"
#include "LooseScreen.generated.h"

UCLASS(Abstract)
class GLITCHUE_API ULooseScreen : public UConditionScreen{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "Buttons", meta = (BindWidget))
	UCustomButton* LoadButton;

	UFUNCTION()
	void LoadSave();
};
