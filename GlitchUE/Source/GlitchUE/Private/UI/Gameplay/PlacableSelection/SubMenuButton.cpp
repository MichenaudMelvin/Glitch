// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/PlacableSelection/SubMenuButton.h"
#include "Components/TextBlock.h"

void USubMenuButton::NativeOnInitialized(){
	Super::NativeOnInitialized();

	Name->SetText(ButtonName);
}

void USubMenuButton::Select(){
	Super::Select();

	for(int i = 0; i < AffectedButtons.Num(); i++){
		AffectedButtons[i]->SetVisibility(ESlateVisibility::Visible);
	}
}

void USubMenuButton::UnSelect(){
	Super::UnSelect();
	
	for(int i = 0; i < AffectedButtons.Num(); i++){
		AffectedButtons[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USubMenuButton::SetAffectedButtons(const TArray<USelectionButton*> NewButtons){
	AffectedButtons = NewButtons;
}
