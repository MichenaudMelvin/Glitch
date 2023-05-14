// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/Tchat/Tchat.h"

#include "Player/UI/Tchat/TchatLineData.h"

void UTchat::NativeConstruct(){
	Super::NativeConstruct();

	PlayAnimation(AppearAnimation, 0, 1, EUMGSequencePlayMode::Forward, AppearanceDuration, false);

	TchatList->SetScrollbarVisibility(ESlateVisibility::Hidden);
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

	GetWorld()->GetTimerManager().SetTimer(DisappearTimer, [&]() {
		IsOpenByUser = false;
		RemoveFromParent();
	}, 1/AppearanceDuration, false);
}

void UTchat::AddTchatLineDelay(){
	UTchatLine* LastWidget = Cast<UTchatLine>(TchatList->GetDisplayedEntryWidgets()[TchatList->GetDisplayedEntryWidgets().Num() - 1]);
	TchatLines.Add(LastWidget);
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
		TchatLines[TchatLines.Num() - 1]->SetLineAsRead(bIsSameSpeaker);

		UTchatLineData* CurrentData = Cast<UTchatLineData>(TchatList->GetItemAt(TchatList->GetNumItems() - 1));
		CurrentData->bIsMessageRead = true;
		CurrentData->bShouldHideSpeaker = bIsSameSpeaker;
	}

	if(!bIsSameSpeaker){
		LastSpeaker = NewSpeaker;
	}

	TchatLine->Message = NewMessage;

	TchatLine->SpeakerColor = FSlateColor(SpeakerColor);

	TchatList->AddItem(TchatLine);

	TchatList->ScrollToBottom();

	// forced to use a timer because the display entry list is not updated instantly
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTchat::AddTchatLineDelay, 0.01f, false);
}
