// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/ConditionScreen/ConditionScreen.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayerController.h"

UConditionScreen::UConditionScreen(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
	MainMenu = FSoftObjectPath("/Game/Maps/Main_Menu");
}

void UConditionScreen::NativeOnInitialized(){
	Super::NativeOnInitialized();

	BackButton->OnClicked.AddDynamic(this, &UConditionScreen::BackToMainMenu);

	AddWidgetToFocusList(BackButton);
}

void UConditionScreen::NativeConstruct(){
	Super::NativeConstruct();

	Cast<AMainPlayerController>(CurrentController)->UnbindAll(true);
	CurrentController->ShowMouseCursor(true, this);
	UGameplayStatics::SetGamePaused(CurrentController, true);
}

void UConditionScreen::BackToMainMenu(){
	UGameplayStatics::OpenLevelBySoftObjectPtr(CurrentController, MainMenu, true, "SkipIntro");
}
