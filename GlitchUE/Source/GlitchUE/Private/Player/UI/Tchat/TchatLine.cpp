// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/Tchat/TchatLine.h"

void UTchatLine::NativeOnListItemObjectSet(UObject* ListItemObject){
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	CurrentData = Cast<UTchatLineData>(ListItemObject);

	SpeakerText->SetText(FText::FromString(CurrentData->Speaker));
	SpeakerText->SetColorAndOpacity(CurrentData->SpeakerColor);

	MessageText->SetText(FText::FromString(CurrentData->Message));

	if(CurrentData->bIsMessageRead){
		MessageText->SetColorAndOpacity(ReadMessageColor);
	}

	if(CurrentData->bShouldHideSpeaker){
		SpeakerText->SetOpacity(0);
	}
}

void UTchatLine::SetLineAsRead(const bool HideSpeaker) const{

	if(HideSpeaker){
		SpeakerText->SetOpacity(0);
	}

	MessageText->SetColorAndOpacity(ReadMessageColor);
}
