// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/PlacableSelection/PlacableSelection.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayerController.h"

void UPlacableSelection::NativeOnInitialized(){
	Super::NativeOnInitialized();

	MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	MainPlayer = Cast<AMainPlayer>(MainPlayerController->GetCharacter());

	ButtonList.Add(NormalModeButton);
	ButtonList.Add(PlacableButton1);
	ButtonList.Add(PlacableButton2);
	ButtonList.Add(PlacableButton3);
	ButtonList.Add(PlacableButton4);
	ButtonList.Add(PlacableButton5);
	ButtonList.Add(PlacableButton6);
	ButtonList.Add(PlacableButton7);
	ButtonList.Add(PlacableButton8);
	ButtonList.Add(DestructionModeButton);
}
