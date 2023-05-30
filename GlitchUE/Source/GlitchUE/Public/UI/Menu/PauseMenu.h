// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Custom/CustomUserWidget.h"
#include "UI/Menu/Saves/SaveSelection.h"
#include "UI/Menu/Settings/SettingsContainerMenu.h"
#include "PauseMenu.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UPauseMenu : public UCustomUserWidget{
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buttons", meta = (BindWidget))
	UCustomButton* ContinueButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buttons", meta = (BindWidget))
	UCustomButton* SaveAndLoadButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buttons", meta = (BindWidget))
	UCustomButton* SettingsButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buttons", meta = (BindWidget))
	UCustomButton* BackButton;

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
