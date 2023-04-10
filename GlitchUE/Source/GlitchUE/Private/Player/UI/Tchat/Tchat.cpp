// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/Tchat/Tchat.h"

#include "Player/UI/Tchat/TchatLineData.h"

void UTchat::NativeConstruct(){
	Super::NativeConstruct();

	PlayAnimation(AppearAnimation, 0, 1, EUMGSequencePlayMode::Forward, AppearanceDuration, false);
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

void UTchat::AddTchatLine(const FString NewLocation, const FString NewSpeaker, const FString NewMessage, const FLinearColor SpeakerColor){
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

	TchatLine->Location = "[" + NewLocation + "]";

	TchatLine->Speaker = " " + NewSpeaker + ": ";

	TchatLine->Message = NewMessage;

	TchatLine->SpeakerColor = FSlateColor(SpeakerColor);

	TchatList->AddItem(TchatLine);
}
