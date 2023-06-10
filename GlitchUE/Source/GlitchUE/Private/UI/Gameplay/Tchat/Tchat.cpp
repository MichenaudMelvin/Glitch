// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/Tchat/Tchat.h"
#include "UI/Gameplay/Tchat/TchatLineData.h"

void UTchat::NativeOnInitialized(){
	Super::NativeOnInitialized();

	AppearanceDuration /= 1;
}

void UTchat::NativeConstruct(){
	Super::NativeConstruct();

	PlayAnimation(AppearAnimation, 0, 1, EUMGSequencePlayMode::Forward, AppearanceDuration, false);

	TchatList->SetScrollbarVisibility(ESlateVisibility::Hidden);
	TchatList->ScrollToBottom();
}

void UTchat::CheckDisappearance(){
	if(IsAnimationPlaying(AppearAnimation)){
		GetWorld()->GetTimerManager().ClearTimer(DisappearTimer);
		PlayAnimation(AppearAnimation, GetAnimationCurrentTime(AppearAnimation), 1, EUMGSequencePlayMode::Forward, AppearanceDuration, false);
	}
}

void UTchat::StartDestructTimer(){
	GetWorld()->GetTimerManager().SetTimer(DestructTimer, this, &UTchat::CloseTchat, DestructTime, false);
}

void UTchat::StopDestructTimer(){
	GetWorld()->GetTimerManager().ClearTimer(DestructTimer);
}

void UTchat::ResetDestructTimer(){
	StopDestructTimer();
	StartDestructTimer();
}

void UTchat::OpenTchat(){
	IsOpenByUser = true;

	if(IsInViewport()){
		StopDestructTimer();
		CheckDisappearance();
		return;
	}

	AddToViewport();
}

void UTchat::CloseTchat(){
	PlayAnimation(AppearAnimation, GetAnimationCurrentTime(AppearAnimation), 1, EUMGSequencePlayMode::Reverse, AppearanceDuration, false);

	//CloseTchatDelay() is call in blueprint
}

void UTchat::CloseTchatDelay(){
	IsOpenByUser = false;
	RemoveFromParent();
}

void UTchat::AddTchatLineDelay(){
	UTchatLineData* TchatLine = NewObject<UTchatLineData>();

	TchatLine->Speaker = CurrentSpeaker + ": ";

	const bool bIsSameSpeaker = LastSpeaker == CurrentSpeaker;

	if(!bIsSameSpeaker){
		LastSpeaker = CurrentSpeaker;
	}

	TchatLine->Message = CurrentMessage;

	TchatLine->SpeakerColor = FSlateColor(CurrentSpeakerColor);

	TchatList->AddItem(TchatLine);

	TchatList->ScrollToBottom();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TchatList, &UListView::ScrollToBottom, 0.001f, false);
}

void UTchat::RebuildList(){
	TchatList->AddItem(LastItem);
}

void UTchat::AddTchatLine(const FString NewSpeaker, const FString NewMessage, const FLinearColor SpeakerColor){
	if(!IsOpenByUser){
		if(!IsInViewport()){
			AddToViewport();
			StartDestructTimer();
		} else{
			CheckDisappearance();
			ResetDestructTimer();
		}
	}

	UTchatLineData* TchatLine = NewObject<UTchatLineData>();

	TchatLine->Speaker = NewSpeaker + ": ";

	const bool bIsSameSpeaker = LastSpeaker == NewSpeaker;

	if(TchatList->GetNumItems() > 0){
		UTchatLineData* CurrentData = Cast<UTchatLineData>(TchatList->GetItemAt(TchatList->GetNumItems() - 1));
		CurrentData->bIsMessageRead = true;
		CurrentData->bShouldHideSpeaker = bIsSameSpeaker;

		LastItem = TchatList->GetListItems()[TchatList->GetNumItems() - 1];

		TchatList->RemoveItem(LastItem);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTchat::RebuildList, 0.001f, false);
	}

	CurrentSpeaker = NewSpeaker;
	CurrentMessage = NewMessage;
	CurrentSpeakerColor = SpeakerColor;

	// forced to use a timer because the display entry list is not updated instantly
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTchat::AddTchatLineDelay, 0.002f, false);
}
