// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/ConditionScreen/LooseScreen.h"
#include "Helpers/FunctionsLibrary/UsefulFunctions.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayerController.h"

void ULooseScreen::NativeOnInitialized(){
	Super::NativeOnInitialized();

	LoadButton->OnClicked.AddDynamic(this, &ULooseScreen::LoadSave);

	AddWidgetToFocusList(LoadButton);
}

void ULooseScreen::NativeConstruct(){
	Super::NativeConstruct();

	IsValid(UUsefulFunctions::LoadSave(UWorldSave::StaticClass(), 0, false)) ? LoadButton->UnblockButton() : LoadButton->BlockButton(true);
}

void ULooseScreen::LoadSave(){
	Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(CurrentController))->GlobalWorldLoad(0);
}
