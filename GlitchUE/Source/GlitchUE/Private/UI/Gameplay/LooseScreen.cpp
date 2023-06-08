// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/LooseScreen.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayerController.h"

void ULooseScreen::NativeOnInitialized(){
	Super::NativeOnInitialized();

	LoadButton->OnClicked.AddDynamic(this, &ULooseScreen::OpenLoadWidget);
	BackButton->OnClicked.AddDynamic(this, &ULooseScreen::BackToMainMenu);

	SaveSelectionWidget = Cast<USaveSelection>(CreateWidget(this, SaveSelectionWidgetClass));

	AddWidgetToFocusList(LoadButton);
	AddWidgetToFocusList(BackButton);
}

void ULooseScreen::NativeConstruct(){
	Super::NativeConstruct();

	Cast<AMainPlayerController>(CurrentController)->UnbindAll(true);
}

void ULooseScreen::OpenLoadWidget(){
	SaveSelectionWidget->OpenSaveSelection();
}

void ULooseScreen::BackToMainMenu(){
	UGameplayStatics::OpenLevelBySoftObjectPtr(CurrentController, MainMenu, true, "SkipIntro");
}
