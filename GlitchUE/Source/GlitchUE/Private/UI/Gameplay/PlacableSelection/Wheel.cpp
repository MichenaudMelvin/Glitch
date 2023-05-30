// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/PlacableSelection/Wheel.h"
#include "PlacableObject/ConstructionZone.h"
#include "Blueprint/WidgetLayoutLibrary.h"
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
}

void UWheel::NativeConstruct(){
	Super::NativeConstruct();

	const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld())/2;

	CurrentController->SetMouseLocation(ViewportSize.X, ViewportSize.Y);

	const bool bIsCurrentSlotOccupied = MainPlayer->GetCurrentConstructionZone()->IsSlotOccupied();

	for(int i = 0; i < ButtonList.Num(); i++){
		ButtonList[i]->UnSelect();
		bIsCurrentSlotOccupied ? ButtonList[i]->UnbindButtons() : ButtonList[i]->BindButtons();

		if(!bIsCurrentSlotOccupied){
			AddWidgetToFocusList(ButtonList[i]);
		}
	}

	const ESlateVisibility TargetVisibility = bIsCurrentSlotOccupied ? ESlateVisibility::Visible : ESlateVisibility::Hidden; 

	DestructButton->SetVisibility(TargetVisibility);

	if(bIsCurrentSlotOccupied){
		AddWidgetToFocusList(DestructButton);
	}

	DestructButton->OnClicked.Clear();
	DestructButton->OnClicked.AddDynamic(MainPlayer->GetCurrentConstructionZone(), &AConstructionZone::DestroyCurrentUnit);
}

void UWheel::FocusWidgets(){
	if(!bIsFocusNeeded){
		return;
	}

	for(int i = 0; i < FocusList.Num(); i++){
		#if WITH_EDITOR
			if(!FocusList[i]->GetClass()->ImplementsInterface(UUIFocus::StaticClass())){
				const auto ClassName = FocusList[i]->GetClass()->GetName();

				GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow,FString::Printf(TEXT("%s does not implement UIFocus interface"), *ClassName));
				UE_LOG(LogTemp, Warning, TEXT("%s does not implement UIFocus interface"), *ClassName);
				continue;
			}
		#endif

		Cast<IUIFocus>(FocusList[i])->UnReceiveFocus();
	}

	for(int i = 0; i < FocusList.Num(); i++){
		#if WITH_EDITOR
		if(!FocusList[i]->GetClass()->ImplementsInterface(UUIFocus::StaticClass())){
			const auto ClassName = FocusList[i]->GetClass()->GetName();

			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow,FString::Printf(TEXT("%s does not implement UIFocus interface"), *ClassName));
			UE_LOG(LogTemp, Warning, TEXT("%s does not implement UIFocus interface"), *ClassName);
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
	MainPlayer->GetPreviewPlacableActor()->SetShouldRangeUpdate(true);
	MainPlayer->GetPreviewPlacableActor()->SetData(nullptr);

	for(int i = 0; i < ButtonList.Num(); i++){
		ButtonList[i]->BindButtons();
		AddWidgetToFocusList(ButtonList[i]);
	}
}

void UWheel::SetDescription(FText NewDescription) const{
	PlacableDescription->SetText(NewDescription);
}
