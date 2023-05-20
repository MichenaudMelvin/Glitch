// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenu.h"
#include "Components/CanvasPanel.h"
#include "Components/ListView.h"
#include "UI/Custom/SubWidget.h"
#include "SettingsContainerMenu.generated.h"

UCLASS(Abstract)
class GLITCHUE_API USettingsContainerMenu : public USubWidget{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TArray<TSubclassOf<USettingsMenu>> SettingsWidgetsClasses;

	UPROPERTY(BlueprintReadOnly)
	TArray<USettingsMenu*> AllSettingsWidgets;

	UPROPERTY(EditDefaultsOnly, Category = "Widget", meta = (BindWidget))
	UListView* SettingsButtonCategories;

	UPROPERTY(EditDefaultsOnly, Category = "Widget", meta = (BindWidget))
	UCanvasPanel* SettingsWidgetContainer;

	UPROPERTY(EditDefaultsOnly, Category = "Widget", meta = (BindWidget))
	UButton* ResetToDefaultButton;

	UPROPERTY(EditDefaultsOnly, Category = "Widget", meta = (BindWidget))
	UButton* ApplySettingsButton;

	USettingsMenu* CurrentShownWidget;

	const FAnchors FullSizeAnchors = FAnchors(0, 0, 1, 1);

	const FMargin FullSizeMargin = FMargin(0, 0, 0, 0);

	void BindButtons();

public:
	void SetCurrentShownWidget(USettingsMenu* NewWidget);
};
