// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/Saves/SaveButton.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/SaveInterface.h"

void USaveButton::NativeConstruct(){
	Super::NativeConstruct();

	SaveButton->OnClicked.AddDynamic(this, &USaveButton::OnSelectSave);
	GameMode = UGameplayStatics::GetGameMode(GetWorld());
}

void USaveButton::NativeOnListItemObjectSet(UObject* ListItemObject){
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	CurrentSave = Cast<UWorldSave>(ListItemObject);

	FSlateBrush Brush;
	Brush.SetResourceObject(CurrentSave->SaveImage);
	SaveButton->WidgetStyle.SetNormal(Brush);
}

void USaveButton::OnSelectSave(){
	SaveSelectionOwner->SetupSaveActions(this);
}

void USaveButton::SetIndex(const int NewIndex){
	Index = NewIndex;
}

void USaveButton::SetOwner(USaveSelection* NewOwner){
	SaveSelectionOwner = NewOwner;
}

void USaveButton::Save(){
	Cast<AGlitchUEGameMode>(GameMode)->GlobalWorldSave(Index);
}

void USaveButton::Load(){
	Cast<ISaveInterface>(GameMode)->GlobalWorldLoad(Index);
}
