// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/Saves/SaveSelection.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Helpers/FunctionsLibrary/UsefulFunctions.h"
#include "UI/Menu/Saves/SaveButton.h"
#include "UI/Menu/Saves/SaveAction.h"
#include "Saves/WorldSave.h"

void USaveSelection::NativeConstruct(){
	Super::NativeConstruct();

	for(int i = 0; i < WorldSaveSlot; i++){
		UAbstractSave* CurrentSave = UUsefulFunctions::LoadSave(UWorldSave::StaticClass(), i);

		SaveListView->AddItem(CurrentSave);
	}

	SaveAction->SetVisibility(ESlateVisibility::Hidden);
}

void USaveSelection::SetupSaveActions(USaveButton* SaveButton) const{
	UCanvasPanelSlot* SaveActionPanelSlot = Cast<UCanvasPanelSlot>(SaveAction->Slot);

	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	SaveAction->BindContextAction(SaveButton);

	SaveAction->SetVisibility(ESlateVisibility::Visible);

	SaveActionPanelSlot->SetPosition(MousePosition);
}

void USaveSelection::CreateEntry(UObject* Item, UUserWidget* Widget){

	const UWorldSave* CurrentSave = Cast<UWorldSave>(Item);
	USaveButton* CurrentSaveButton = Cast<USaveButton>(Widget);

	#if WITH_EDITOR

	// call la fonction dans l'éditeur donc sécurité
	if(!IsValid(CurrentSave) || !IsValid(CurrentSaveButton)){
		return;
	}

	#endif

	CurrentSaveButton->SetIndex(CurrentSave->Index);
	CurrentSaveButton->SetOwner(this);
}

void USaveSelection::OpenSaveSelection(const bool bAllowSave){
	SaveAction->UseSaveButton(bAllowSave);
	AddToViewport();
}
