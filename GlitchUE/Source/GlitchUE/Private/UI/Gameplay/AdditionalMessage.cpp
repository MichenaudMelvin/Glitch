// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/AdditionalMessage.h"
#include "Components/TextBlock.h"

void UAdditionalMessage::AddMessageToScreen(const FString NewMessage){
	if(!IsVisible()){
		AddToViewport();
	}

	AdditionalMessageText->SetText(FText::FromString(NewMessage));
	PlayAnimation(AdditionalMessageAnim, 0, 1, EUMGSequencePlayMode::Forward, AdditionalMessageAnimSpeed, true);

	// remove from parent will be call by the animation, otherwise it can crash the editor with a timer
}
