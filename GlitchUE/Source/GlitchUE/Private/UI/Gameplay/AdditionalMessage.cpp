// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/AdditionalMessage.h"
#include "Components/TextBlock.h"

void UAdditionalMessage::AddMessageToScreen(const FString NewMessage){
	IsVisible() ? GetWorld()->GetTimerManager().ClearTimer(AnimTimerHandle) : AddToViewport();

	AdditionalMessageText->SetText(FText::FromString(NewMessage));
	PlayAnimation(AdditionalMessageAnim, 0, 1, EUMGSequencePlayMode::Forward, AdditionalMessageAnimSpeed, true);

	GetWorld()->GetTimerManager().SetTimer(AnimTimerHandle, [&](){
		RemoveFromParent();
	}, 1/AdditionalMessageAnimSpeed, false);
}
