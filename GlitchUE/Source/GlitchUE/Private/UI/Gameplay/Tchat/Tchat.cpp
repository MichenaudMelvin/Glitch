// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/Tchat/Tchat.h"

#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayerController.h"
#include "UI/Gameplay/Tchat/TchatLineData.h"

void UTchat::NativeOnInitialized(){
	Super::NativeOnInitialized();

	AppearanceDuration /= 1;
	ExtendDuration /= 1;

	//AddWidgetToFocusList(TchatList);
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
	bIsOpenByUser = true;

	PlayAnimation(ExtendTchatAnim, 0, 1, EUMGSequencePlayMode::Forward, ExtendDuration, false);

	AMainPlayerController* CastedController = Cast<AMainPlayerController>(CurrentController);
	CastedController->UnbindAll();
	CastedController->BindOpenTchat();
	CastedController->GetPlayerStatsWidget()->RemoveFromParent();

	if(CastedController->GetTimerWidget()->IsTimerRunning()){
		CastedController->GetTimerWidget()->RemoveFromParent();
	}

	UGameplayStatics::SetGamePaused(CurrentController,true);

	if(IsInViewport()){
		StopDestructTimer();
		CheckDisappearance();
		return;
	}

	AddToViewport();
}

void UTchat::CloseTchat(){
	PlayAnimation(AppearAnimation, GetAnimationCurrentTime(AppearAnimation), 1, EUMGSequencePlayMode::Reverse, AppearanceDuration, false);
	if(bIsOpenByUser){
		bIsOpenByUser = false;
		PlayAnimation(ExtendTchatAnim, 0, 1, EUMGSequencePlayMode::Reverse, ExtendDuration, false);
		UGameplayStatics::SetGamePaused(CurrentController,false);

		AMainPlayerController* CastedController = Cast<AMainPlayerController>(CurrentController);

		CastedController->BindNormalMode();
		CastedController->GetPlayerStatsWidget()->AddToViewport();

		if(CastedController->GetTimerWidget()->IsTimerRunning()){
			CastedController->GetTimerWidget()->AddToViewport();
		}
	}

	//RemoveFromParent() is call in blueprint
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

	AllTchatLines.Add(FTchatStruct(CurrentSpeaker, CurrentMessage, CurrentSpeakerColor, 0));
	TchatList->AddItem(TchatLine);

	TchatList->ScrollToBottom();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TchatList, &UListView::ScrollToBottom, 0.001f, false);
}

void UTchat::RebuildList() const{
	TchatList->AddItem(LastItem);
}

void UTchat::WriteMessageList(){
	AddTchatLine(CurrentListToAdd[0].Speaker, CurrentListToAdd[0].TextMessage, CurrentListToAdd[0].SpeakerColor);

	if(CurrentListToAdd.Num() == 1){
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(MultipleMessagesTimerHandle, this, &UTchat::WriteMessageList, CurrentListToAdd[0].DelayForNextMessage, false);
	CurrentListToAdd.RemoveAt(0);
}

void UTchat::AddTchatLine(const FString NewSpeaker, const FString NewMessage, const FLinearColor SpeakerColor){
	if(!bIsOpenByUser){
		if(!IsInViewport()){
			if(!Cast<AMainPlayerController>(CurrentController)->IsWheelOpened()){
				AddToViewport();
				StartDestructTimer();
			}

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

void UTchat::AddMultipleTchatLines(TArray<FTchatStruct> TchatLines){
	for(int i = 0; i < TchatLines.Num(); i++){
		CurrentListToAdd.Add(TchatLines[i]);
	}

	WriteMessageList();
}

bool UTchat::IsOpenByUser() const{
	return bIsOpenByUser;
}

TArray<FTchatStruct> UTchat::GetAllTchatLines() const{
	return AllTchatLines;
}

void UTchat::AddTchatLineWithATchatStruct(FTchatStruct TchatStruct){
	UTchatLineData* TchatLine = NewObject<UTchatLineData>();

	TchatLine->Speaker = TchatStruct.Speaker + ": ";

	TchatLine->Message = TchatStruct.TextMessage;

	TchatLine->SpeakerColor = FSlateColor(TchatStruct.SpeakerColor);

	AllTchatLines.Add(TchatStruct);
	TchatList->AddItem(TchatLine);
}
