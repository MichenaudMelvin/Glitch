// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Menu/Saves/SaveSelection.h"
#include "UI/Menu/Settings/SettingsContainerMenu.h"
#include "PauseMenu.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UPauseMenu : public UUserWidget{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	UPROPERTY()
	USaveSelection* SaveSelectionWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Class")
	TSubclassOf<USaveSelection> SaveSelectionWidgetClass;

	UPROPERTY()
	USettingsContainerMenu* SettingsWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Class")
	TSubclassOf<USettingsContainerMenu> SettingsWidgetClass;

	AMainPlayerController* MainPlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "Buttons", meta = (BindWidget))
	UButton* ContinueButton;

	UPROPERTY(EditDefaultsOnly, Category = "Buttons", meta = (BindWidget))
	UButton* SaveAndLoadButton;

	UPROPERTY(EditDefaultsOnly, Category = "Buttons", meta = (BindWidget))
	UButton* SettingsButton;

	UPROPERTY(EditDefaultsOnly, Category = "Buttons", meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(EditDefaultsOnly, Category = "MainMenu")
	TSoftObjectPtr<UWorld> MainMenu;

	UFUNCTION()
	void ContinueGame();

	UFUNCTION()
	void OpenSaveMenu();

	UFUNCTION()
	void OpenSettings();

	UFUNCTION()
	void BackToMainMenu();
};
