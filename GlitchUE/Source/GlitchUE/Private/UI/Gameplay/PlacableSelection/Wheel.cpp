// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/PlacableSelection/Wheel.h"
#include "PlacableObject/ConstructionZone.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "PlacableObject/TurretData.h"
#include "Player/MainPlayerController.h"

void UWheel::NativeOnInitialized(){
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

	PlacableButton1->SetWheel(this);
	PlacableButton2->SetWheel(this);
	PlacableButton3->SetWheel(this);
	PlacableButton4->SetWheel(this);
	PlacableButton5->SetWheel(this);
	PlacableButton6->SetWheel(this);
	PlacableButton7->SetWheel(this);
	PlacableButton8->SetWheel(this);

	bIsDynamicFocusList = true;
}

void UWheel::NativeConstruct(){
	Super::NativeConstruct();

	const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld())/2;

	CurrentController->SetMouseLocation(ViewportSize.X, ViewportSize.Y);

	const bool bIsCurrentSlotOccupied = MainPlayer->GetCurrentConstructionZone()->IsSlotOccupied();

	for(int i = 0; i < ButtonList.Num(); i++){
		ButtonList[i]->UnSelect();
		bIsCurrentSlotOccupied ? ButtonList[i]->UnbindButtons() : ButtonList[i]->BindButtons();

		bIsCurrentSlotOccupied ? ButtonList[i]->SetVisibility(ESlateVisibility::HitTestInvisible) : AddWidgetToFocusList(ButtonList[i]); 
	}

	const ESlateVisibility TargetVisibility = bIsCurrentSlotOccupied ? ESlateVisibility::Visible : ESlateVisibility::Hidden; 

	DestructButton->SetVisibility(TargetVisibility);

	if(bIsCurrentSlotOccupied){
		AddWidgetToFocusList(DestructButton);
		SetDescription(MainPlayer->GetCurrentConstructionZone()->GetUnit()->GetData());
	}

	DestructButton->OnClicked.Clear();
	DestructButton->OnClicked.AddDynamic(MainPlayer->GetCurrentConstructionZone(), &AConstructionZone::DestroyCurrentUnit);
	DestructButton->OnClicked.AddDynamic(this, &UWheel::ClickOnDestructButton);
}

void UWheel::FocusWidgets(){
	if(!bIsFocusNeeded){
		return;
	}

	for(int i = 0; i < FocusList.Num(); i++){

		#if WITH_EDITOR
		if(!CheckValidity(FocusList[i])){
			continue;
		}
		#endif

		Cast<IUIFocus>(FocusList[i])->UnReceiveFocus();
	}

	for(int i = 0; i < FocusList.Num(); i++){
		#if WITH_EDITOR
		if(!CheckValidity(FocusList[i])){
			continue;
		}
		#endif

		if(FocusList[i]->HasKeyboardFocus()){
			Cast<IUIFocus>(FocusList[i])->ReceiveFocus();
			LastFocusWidgetIndex = i;
		}
	}
}

void UWheel::ClickOnDestructButton(){
	RemoveWidgetToFocusList(DestructButton);

	DestructButton->SetVisibility(ESlateVisibility::Hidden);
}

void UWheel::ClickOnDestructButtonDelay(){
	MainPlayer->GetPreviewPlacableActor()->SetShouldRangeUpdate(true);
	MainPlayer->GetPreviewPlacableActor()->SetData(nullptr);

	for(int i = 0; i < ButtonList.Num(); i++){
		ButtonList[i]->BindButtons();
		AddWidgetToFocusList(ButtonList[i]);
		ButtonList[i]->SetVisibility(ESlateVisibility::Visible);
	}

	if(CurrentController->IsUsingGamepad()){
		Refocus();
	}
}

void UWheel::SetDescription_Implementation(const UPlacableActorData* Data) const{
	if(!IsValid(Data)){
		DescriptionPanel->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	DescriptionPanel->SetVisibility(ESlateVisibility::Visible);

	DescriptionText->SetText(Data->Description);

	DamagesText->SetText(FText::AsNumber(Data->Damages));
	AttackSpeedText->SetText(FText::AsNumber(Data->AttackRate));
	RangeText->SetText(FText::AsNumber(Data->AttackRange));

	// this one is only working on blueprint
	// FString WallBoolean = "✗";
	//
	// if(Data->IsA(UTurretData::StaticClass())){
	// 	if(Cast<UTurretData>(Data)->CanSeeThroughWalls){
	// 		WallBoolean = "✓";
	// 	}
	// }
	//
	// WallText->SetText(FText::FromString(WallBoolean));
}