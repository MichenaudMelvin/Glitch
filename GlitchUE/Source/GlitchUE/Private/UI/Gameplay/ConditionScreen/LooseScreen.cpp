// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/ConditionScreen/LooseScreen.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayerController.h"

void ULooseScreen::NativeOnInitialized(){
	Super::NativeOnInitialized();

	LoadButton->OnClicked.AddDynamic(this, &ULooseScreen::OpenLoadWidget);

	SaveSelectionWidget = Cast<USaveSelection>(CreateWidget(this, SaveSelectionWidgetClass));

	AddWidgetToFocusList(LoadButton);
}

void ULooseScreen::OpenLoadWidget(){
	SaveSelectionWidget->OpenSaveSelection();
}
