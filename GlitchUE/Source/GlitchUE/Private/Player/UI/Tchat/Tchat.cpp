// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/Tchat/Tchat.h"

#include "Player/UI/Tchat/TchatLineData.h"

void UTchat::NativeConstruct(){
	Super::NativeConstruct();

	PlayAnimation(AppearAnimation, 0, 1, EUMGSequencePlayMode::Forward, 2, false);
}

void UTchat::StartDestructTimer(){
	GetWorld()->GetTimerManager().SetTimer(DestructTimer, [&]() {
		CloseTchat();
	}, DestructTime, false);
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
		return;
	}

	AddToViewport();
}

void UTchat::CloseTchat(){
	PlayAnimation(AppearAnimation, 0, 1, EUMGSequencePlayMode::Reverse, 2, false);

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		IsOpenByUser = false;
		RemoveFromParent();
	}, AppearAnimation->GetEndTime(), false);
}

void UTchat::AddTchatLine(const FString NewLocation, const FString NewSpeaker, const FString NewMessage, const FLinearColor SpeakerColor){
	if(!IsOpenByUser){
		if(!IsInViewport()){
			AddToViewport();
			StartDestructTimer();
		} else{
			ResetDestructTimer();
		}
	}

	UTchatLineData* Salut = NewObject<UTchatLineData>();

	Salut->Location = "[" + NewLocation + "]";

	Salut->Speaker = " " + NewSpeaker + ": ";

	Salut->Message = NewMessage;

	Salut->SpeakerColor = FSlateColor(SpeakerColor);

	// const FString Place = "[" + NewPlace + "]";
	//
	// const FString Speaker = " " + NewSpeaker + ": ";
	//
	// const FString Message = NewMessage;

	TchatList->AddItem(Salut);

	// const FString FullMessage = TchatList->GetText().ToString() == "" ? Place + Speaker + Message : TchatList->GetText().ToString() + "\n" + Place + Speaker + Message;
	//
	// TchatList->SetText(FText::FromString(FullMessage));
}
