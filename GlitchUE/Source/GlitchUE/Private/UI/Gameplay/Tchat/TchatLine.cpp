// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/Tchat/TchatLine.h"

void UTchatLine::NativeOnListItemObjectSet(UObject* ListItemObject){
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	CurrentData = Cast<UTchatLineData>(ListItemObject);

	SpeakerText->SetText(FText::FromString(CurrentData->Speaker));
	SpeakerText->SetColorAndOpacity(CurrentData->SpeakerColor);

	MessageText->SetText(FText::FromString(CurrentData->Message));
}
