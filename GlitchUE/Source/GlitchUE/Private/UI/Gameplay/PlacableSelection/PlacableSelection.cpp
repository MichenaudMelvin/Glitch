// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/PlacableSelection/PlacableSelection.h"
#include "Player/MainPlayerController.h"

void UPlacableSelection::NativeOnInitialized(){
	Super::NativeOnInitialized();

	MainPlayer = Cast<AMainPlayer>(CurrentController->GetCharacter());

	ButtonList.Add(PlacableButton1);
	ButtonList.Add(PlacableButton2);
	ButtonList.Add(PlacableButton3);
	ButtonList.Add(PlacableButton4);
	ButtonList.Add(PlacableButton5);
	ButtonList.Add(PlacableButton6);
	ButtonList.Add(PlacableButton7);
	ButtonList.Add(PlacableButton8);

	AddWidgetToFocusList(PlacableButton1);
	AddWidgetToFocusList(PlacableButton2);
	AddWidgetToFocusList(PlacableButton3);
	AddWidgetToFocusList(PlacableButton4);
	AddWidgetToFocusList(PlacableButton5);
	AddWidgetToFocusList(PlacableButton6);
	AddWidgetToFocusList(PlacableButton7);
	AddWidgetToFocusList(PlacableButton8);
}
