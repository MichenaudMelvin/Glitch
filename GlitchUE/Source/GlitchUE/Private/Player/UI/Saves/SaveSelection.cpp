// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/Saves/SaveSelection.h"
#include "Components/CanvasPanelSlot.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "Player/UI/Saves/SaveButton.h"
#include "Player/UI/Saves/SaveAction.h"
#include "Saves/WorldSave.h"

void USaveSelection::NativeConstruct(){
	Super::NativeConstruct();

	for(int i = 0; i < WorldSaveSlot; i++){
		UAbstractSave* CurrentSave = UUsefullFunctions::LoadSave(UWorldSave::StaticClass(), i);

		SaveListView->AddItem(CurrentSave);
	}
}

void USaveSelection::SetupSaveActions(USaveButton* SaveButton) const{
	UCanvasPanelSlot* SaveActionPanelSlot = Cast<UCanvasPanelSlot>(SaveAction->Slot);
	// const UCanvasPanelSlot* SaveButtonPanelSlot = Cast<UCanvasPanelSlot>(SaveButton->Slot);

	SaveAction->BindContextAction(SaveButton);

	SaveAction->SetVisibility(ESlateVisibility::Visible);

	// SaveActionPanelSlot->SetPosition(SaveButtonPanelSlot->GetPosition());
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
