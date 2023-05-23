// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/PlacableSelection/Wheel.h"
#include "PlacableObject/ConstructionZone.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Player/MainPlayerController.h"

void UWheel::NativeOnInitialized(){
	Super::NativeOnInitialized();

	TArray<USelectionButton*> TurretsList;
	TurretsList.Add(PlacableButton1);
	TurretsList.Add(PlacableButton2);
	TurretsList.Add(PlacableButton3);
	TurretsList.Add(PlacableButton4);

	PlacableButton1->SetWheel(this);
	PlacableButton2->SetWheel(this);
	PlacableButton3->SetWheel(this);
	PlacableButton4->SetWheel(this);

	TArray<USelectionButton*> TrapsList;
	TrapsList.Add(PlacableButton5);
	TrapsList.Add(PlacableButton6);
	TrapsList.Add(PlacableButton7);
	TrapsList.Add(PlacableButton8);

	PlacableButton5->SetWheel(this);
	PlacableButton6->SetWheel(this);
	PlacableButton7->SetWheel(this);
	PlacableButton8->SetWheel(this);
}

void UWheel::NativeConstruct(){
	Super::NativeConstruct();

	const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld())/2;

	MainPlayerController->SetMouseLocation(ViewportSize.X, ViewportSize.Y);

	const bool bIsCurrentSlotOccupied = MainPlayer->GetCurrentConstructionZone()->IsSlotOccupied();

	for(int i = 0; i < ButtonList.Num(); i++){
		ButtonList[i]->UnSelect();
		bIsCurrentSlotOccupied ? ButtonList[i]->UnbindButtons() : ButtonList[i]->BindButtons();
	}

	const ESlateVisibility TargetVisibility = bIsCurrentSlotOccupied ? ESlateVisibility::Visible : ESlateVisibility::Hidden; 

	DestructButton->SetVisibility(TargetVisibility);

	DestructButton->OnClicked.Clear();
	DestructButton->OnClicked.AddDynamic(MainPlayer->GetCurrentConstructionZone(), &AConstructionZone::DestroyCurrentUnit);
}

void UWheel::ClickOnDestructButton(){
	DestructButton->SetVisibility(ESlateVisibility::Hidden);

	for(int i = 0; i < ButtonList.Num(); i++){
		ButtonList[i]->BindButtons();
	}
}

void UWheel::SetDescription(FText NewDescription) const{
	PlacableDescription->SetText(NewDescription);
}
