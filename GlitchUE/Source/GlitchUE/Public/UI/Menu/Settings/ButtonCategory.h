// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ButtonCategoryData.h"
#include "SettingsContainerMenu.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ButtonCategory.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UButtonCategory : public UUserWidget, public IUserObjectListEntry{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	USettingsContainerMenu* SettingsContainer;

	UButtonCategoryData* CurrentData;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UCustomButton* Button;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* CategoryText;

	USettingsMenu* WidgetToShow;

	int Index = 0;

	UFUNCTION()
	void SelectCategory();

public:
	UCustomButton* GetButton() const;
};
