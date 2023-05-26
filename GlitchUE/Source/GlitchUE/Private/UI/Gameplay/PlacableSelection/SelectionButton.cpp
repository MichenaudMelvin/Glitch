// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/PlacableSelection/SelectionButton.h"
#include "Components/Button.h"
#include "Player/MainPlayerController.h"
#include "Kismet/GameplayStatics.h"

void USelectionButton::NativeOnInitialized(){
	Super::NativeOnInitialized();

	OriginalColor = Image->BackgroundColor;

	MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	BindButtons();
}

void USelectionButton::Select(){
	Image->SetBackgroundColor(FLinearColor::Green);
}

void USelectionButton::UnSelect(){
	Image->SetBackgroundColor(OriginalColor);
}

void USelectionButton::BindButtons(){
	UnbindButtons();
	Image->OnHovered.AddDynamic(this, &USelectionButton::Select);
	Image->OnUnhovered.AddDynamic(this, &USelectionButton::UnSelect);
}

void USelectionButton::UnbindButtons(){
	Image->OnHovered.Clear();
	Image->OnUnhovered.Clear();
}
