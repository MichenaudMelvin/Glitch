// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubMenuButton.h"
#include "Player/UI/PlacableSelection/PlacableSelection.h"
#include "Wheel.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UWheel : public UPlacableSelection{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USubMenuButton* TurretSubMenuButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USubMenuButton* TrapSubMenuButton;

	TArray<USelectionButton*> MainButtonList;

	USelectionButton* SelectedButton;

	void RadialSelection();
};
