// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/Tchat/Tchat.h"

#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayerController.h"
#include "Player/TransitionPlayerController.h"
#include "UI/Gameplay/Tchat/TchatLineData.h"

void UTchat::NativeOnInitialized(){
	Super::NativeOnInitialized();

	AppearanceDuration /= 1;
	ExtendDuration /= 1;

	CurrentController->OnSwitchToKeyboard.AddDynamic(this, &UTchat::SetKeyboardInstruction);
	CurrentController->OnSwitchToGamepad.AddDynamic(this, &UTchat::SetGamepadInstruction);

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
	} else if(TchatList->RenderOpacity <= 0){
		PlayAnimation(AppearAnimation, 0, 1, EUMGSequencePlayMode::Forward, AppearanceDuration, false);
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

void UTchat::SetKeyboardInstruction(){
	InstructionText->SetText(FText::FromString(GlobalInstruction + " " + KeyboardButton));
}

void UTchat::SetGamepadInstruction(){
	InstructionText->SetText(FText::FromString(GlobalInstruction + " " + GamepadButton));
}

void UTchat::OpenTchat(){
	if(AllTchatLines.Num() == 0){
		return;
	}

	bIsOpenByUser = true;

	PlayAnimation(ExtendTchatAnim, 0, 1, EUMGSequencePlayMode::Forward, ExtendDuration, false);

	CurrentController->IsUsingGamepad() ? SetGamepadInstruction() : SetKeyboardInstruction();

	if(CurrentController->IsA(AMainPlayerController::StaticClass())){
		AMainPlayerController* CastedController = Cast<AMainPlayerController>(CurrentController);
		CastedController->UnbindAll(true);
		CastedController->BindOpenTchat();
		CastedController->GetPlayerStatsWidget()->RemoveFromParent();

		if(CastedController->GetTimerWidget()->IsTimerRunning()){
			CastedController->GetTimerWidget()->RemoveFromParent();
		}

		UGameplayStatics::SetGamePaused(CurrentController,true);
	}

	if(IsInViewport()){
		StopDestructTimer();
		CheckDisappearance();
		return;
	}

	AddToViewport();
}

void UTchat::CloseTchat(){
	bRequestManualScroll = false;

	if(!bIsOpenByUser){
		PlayAnimation(AppearAnimation, GetAnimationCurrentTime(AppearAnimation), 1, EUMGSequencePlayMode::Reverse, AppearanceDuration, true);
		return;
	}

	bIsOpenByUser = false;
	PlayAnimation(ExtendTchatAnim, 0, 1, EUMGSequencePlayMode::Reverse, ExtendDuration, false);

	if(CurrentController->IsA(AMainPlayerController::StaticClass())){
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

void UTchat::WriteMessageList_Implementation(){
	AddTchatLine(CurrentListToAdd[0].Speaker, CurrentListToAdd[0].TextMessage, CurrentListToAdd[0].SpeakerColor);

	if(CurrentListToAdd.Num() == 1){
		OnFinishWritingMessageList.Broadcast();
		CurrentListToAdd.RemoveAt(0);
		bWritingMultipleMessages = false;
	}
}

void UTchat::GamepadScroll(float Axis){
	const int IntAxis = FMath::RoundToInt(Axis);

	if(IntAxis == 0){
		return;
	}

	bRequestManualScroll = true;

	const int ScrollValue = (ScrollMultiplier * -IntAxis) + TargetScrollIndex;

	TargetScrollIndex = FMath::Clamp(ScrollValue, 0, AllTchatLines.Num() - 1);
	TchatList->ScrollIndexIntoView(TargetScrollIndex);
}

void UTchat::MouseScroll(float Axis){
	bRequestManualScroll = true;
}

void UTchat::AddTchatLine(const FString NewSpeaker, const FString NewMessage, const FLinearColor SpeakerColor){
	if(!bIsOpenByUser){
		if(!IsInViewport()){
			if(CurrentController->IsA(AMainPlayerController::StaticClass())){
				if(!Cast<AMainPlayerController>(CurrentController)->IsWheelOpened()){
					AddToViewport();
					StartDestructTimer();
				}
			} else if(CurrentController->IsA(ATransitionPlayerController::StaticClass())){
				OpenTchat();
			}

		} else{
			if(CurrentController->IsA(AMainPlayerController::StaticClass())){
				CheckDisappearance();
				ResetDestructTimer();
			}
		}
	}

	UTchatLineData* TchatLine = NewObject<UTchatLineData>();

	TchatLine->Speaker = NewSpeaker + ": ";

	TchatLine->Message = NewMessage;
	TchatLine->SpeakerColor = SpeakerColor;

	AllTchatLines.Add(FTchatStruct(TchatLine->Speaker, TchatLine->Message, SpeakerColor, 0));
	TchatList->AddItem(TchatLine);

	if(!bRequestManualScroll){
		TchatList->ScrollToBottom();
		TargetScrollIndex = AllTchatLines.Num() - 1;
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TchatList, &UListView::ScrollToBottom, 0.001f, false);
}

void UTchat::AddEmptyTchatLine(){
	UTchatLineData* EmptyLine = NewObject<UTchatLineData>();

	EmptyLine->Speaker = "";
	EmptyLine->Message = "";
	EmptyLine->SpeakerColor = FLinearColor::Black;

	AllTchatLines.Add(FTchatStruct("", "", FLinearColor::Black, 0));
	TchatList->AddItem(EmptyLine);

	TchatList->ScrollToBottom();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TchatList, &UListView::ScrollToBottom, 0.001f, false);
}

void UTchat::AddMultipleTchatLines(TArray<FTchatStruct> TchatLines){
	for(int i = 0; i < TchatLines.Num(); i++){
		CurrentListToAdd.Add(TchatLines[i]);
	}

	if(bWritingMultipleMessages){
		return;
	}

	bWritingMultipleMessages = true;
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
