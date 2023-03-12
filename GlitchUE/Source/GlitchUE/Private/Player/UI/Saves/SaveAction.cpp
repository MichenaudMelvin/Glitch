// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/Saves/SaveAction.h"

void USaveAction::ResetBinds() const{
	SaveButton->OnClicked.Clear();
	LoadButton->OnClicked.Clear();
}

void USaveAction::BindContextAction(USaveButton* SaveButtonReference) const{
	ResetBinds();

	SaveButton->OnClicked.AddDynamic(SaveButtonReference, &USaveButton::Save);
	LoadButton->OnClicked.AddDynamic(SaveButtonReference, &USaveButton::Load);
}
