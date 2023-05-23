// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Gameplay/PlacableSelection/PlacableSelection.h"
#include "Wheel.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UWheel : public UPlacableSelection{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	TArray<USelectionButton*> MainButtonList;

	USelectionButton* SelectedButton;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UButton* DestructButton;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UTextBlock* PlacableDescription;

public:
	void ClickOnDestructButton();

	void SetDescription(const FText NewDescription) const;

};
