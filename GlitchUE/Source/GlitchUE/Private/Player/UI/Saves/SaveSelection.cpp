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

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		for(int i = 0; i < WorldSaveSlot; i++){
			USaveButton* CurrentSaveButton = Cast<USaveButton>(SaveListView->GetDisplayedEntryWidgets()[i]);
			CurrentSaveButton->SetIndex(i);
			CurrentSaveButton->SetOwner(this);
		}
	}, 0.2f, false);
}

void USaveSelection::SetupSaveActions(USaveButton* SaveButton) const{
	UCanvasPanelSlot* SaveActionPanelSlot = Cast<UCanvasPanelSlot>(SaveAction->Slot);
	// const UCanvasPanelSlot* SaveButtonPanelSlot = Cast<UCanvasPanelSlot>(SaveButton->Slot);

	SaveAction->BindContextAction(SaveButton);

	SaveAction->SetVisibility(ESlateVisibility::Visible);

	// SaveActionPanelSlot->SetPosition(SaveButtonPanelSlot->GetPosition());
}
