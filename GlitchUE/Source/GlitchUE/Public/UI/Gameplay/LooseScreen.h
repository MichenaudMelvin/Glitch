// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Custom/CustomButton.h"
#include "UI/Custom/CustomUserWidget.h"
#include "UI/Menu/Saves/SaveSelection.h"
#include "LooseScreen.generated.h"

UCLASS(Abstract)
class GLITCHUE_API ULooseScreen : public UCustomUserWidget{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "Buttons", meta = (BindWidget))
	UCustomButton* LoadButton;

	UFUNCTION()
	void OpenLoadWidget();

	UFUNCTION()
	void BackToMainMenu();

	UPROPERTY(EditDefaultsOnly, Category = "Buttons", meta = (BindWidget))
	UCustomButton* BackButton;

	UPROPERTY()
	USaveSelection* SaveSelectionWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<USaveSelection> SaveSelectionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "MainMenu")
	TSoftObjectPtr<UWorld> MainMenu;
};
