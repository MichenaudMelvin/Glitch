// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/PauseMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayerController.h"

UPauseMenu::UPauseMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
	MainMenu = FSoftObjectPath("/Game/Maps/Main_Menu");
}

void UPauseMenu::NativeOnInitialized(){
	Super::NativeOnInitialized();

	ContinueButton->OnClicked.AddDynamic(this, &UPauseMenu::ContinueGame);
	SaveButton->OnClicked.AddDynamic(this, &UPauseMenu::Save);
	LoadButton->OnClicked.AddDynamic(this, &UPauseMenu::Load);
	SettingsButton->OnClicked.AddDynamic(this, &UPauseMenu::OpenSettings);
	BackButton->OnClicked.AddDynamic(this, &UPauseMenu::BackToMainMenu);

	MainPlayerController = Cast<AMainPlayerController>(CurrentController);

	SettingsWidget = Cast<USettingsContainerMenu>(CreateWidget(this, SettingsWidgetClass));
	SettingsWidget->SetOwnerWidget(this);

	GameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(MainPlayerController));

	AddWidgetToFocusList(ContinueButton);
	AddWidgetToFocusList(SaveButton);
	AddWidgetToFocusList(LoadButton);
	AddWidgetToFocusList(SettingsButton);
	AddWidgetToFocusList(BackButton);
}

void UPauseMenu::NativeConstruct(){
	Super::NativeConstruct();

	MainPlayerController->UnbindPause();
	MainPlayerController->OnPause.AddDynamic(MainPlayerController, &AMainPlayerController::PauseGame);

	MainPlayerController->CanSave() ? SaveButton->UnblockButton() : SaveButton->BlockButton(true);
}

void UPauseMenu::ContinueGame(){
	MainPlayerController->PauseGame();
}

void UPauseMenu::Save(){
	GameMode->GlobalWorldSave(0);
}

void UPauseMenu::Load(){
	GameMode->GlobalWorldLoad(0);
}

void UPauseMenu::OpenSettings(){
	SettingsWidget->AddToViewport();
}

void UPauseMenu::BackToMainMenu(){
	UGameplayStatics::OpenLevelBySoftObjectPtr(MainPlayerController, MainMenu, true, "SkipIntro");
}
