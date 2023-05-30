// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/PauseMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayerController.h"

void UPauseMenu::NativeOnInitialized(){
	Super::NativeOnInitialized();

	ContinueButton->OnClicked.AddDynamic(this, &UPauseMenu::ContinueGame);
	SaveAndLoadButton->OnClicked.AddDynamic(this, &UPauseMenu::OpenSaveMenu);
	SettingsButton->OnClicked.AddDynamic(this, &UPauseMenu::OpenSettings);
	BackButton->OnClicked.AddDynamic(this, &UPauseMenu::BackToMainMenu);

	MainPlayerController = Cast<AMainPlayerController>(CurrentController);

	SaveSelectionWidget = Cast<USaveSelection>(CreateWidget(this, SaveSelectionWidgetClass));
	SaveSelectionWidget->SetOwnerWidget(this);

	SettingsWidget = Cast<USettingsContainerMenu>(CreateWidget(this, SettingsWidgetClass));
	SettingsWidget->SetOwnerWidget(this);

	AddWidgetToFocusList(ContinueButton);
	AddWidgetToFocusList(SaveAndLoadButton);
	AddWidgetToFocusList(SettingsButton);
	AddWidgetToFocusList(BackButton);
}

void UPauseMenu::NativeConstruct(){
	Super::NativeConstruct();

	MainPlayerController->UnbindPause();
	MainPlayerController->OnPause.AddDynamic(MainPlayerController, &AMainPlayerController::PauseGame);
}

void UPauseMenu::ContinueGame(){
	MainPlayerController->PauseGame();
}

void UPauseMenu::OpenSaveMenu(){
	SaveSelectionWidget->OpenSaveSelection(MainPlayerController->CanSave());
}

void UPauseMenu::OpenSettings(){
	SettingsWidget->AddToViewport();
}

void UPauseMenu::BackToMainMenu(){
	UGameplayStatics::OpenLevelBySoftObjectPtr(MainPlayerController, MainMenu, true, "SkipIntro");
}
