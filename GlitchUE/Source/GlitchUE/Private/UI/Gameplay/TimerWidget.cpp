// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/TimerWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayerController.h"

void UTimerWidget::NativeOnInitialized(){
	Super::NativeOnInitialized();

	MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void UTimerWidget::NativeConstruct(){
	Super::NativeConstruct();

	PlayAnimation(Fade, 0, 1, EUMGSequencePlayMode::Forward, FadeTime);
}

void UTimerWidget::RemoveWidget(){
	PlayAnimation(Fade, 0, 1, EUMGSequencePlayMode::Reverse, FadeTime);

	// remove from parent will be call in blueprint
}

FString UTimerWidget::SelectString(const int IntValue){
	FString ConvertedInt = FString::FromInt(IntValue);

	if(IntValue < 10){
		ConvertedInt = "0" + ConvertedInt;
	}

	return ConvertedInt;
}

void UTimerWidget::UpdateTimer(){
	CurrentDisplayTime -= 0.1f;

	FString MinutesValue;

	FString SecondsValue;

	GetTimerInString(CurrentDisplayTime, MinutesValue, SecondsValue);

	TimerText->SetText(FText::FromString(MinutesValue + ":" + SecondsValue));

	if(GetMinutes(CurrentDisplayTime) <= 0 && GetSeconds(CurrentDisplayTime) <= 0){
		FinishTimer(RemoveWidgetAtEnd);
	}
}

void UTimerWidget::GetTimerInString(const float Timer, FString& Minutes, FString& Seconds){
	Minutes = SelectString(GetMinutes(Timer));
	Seconds = SelectString(GetSeconds(Timer));
}

int UTimerWidget::GetTimerInInt(const float Time) const{
	return FMath::RoundToInt(Time);
}

int UTimerWidget::GetSeconds(const float Time) const{
	return GetTimerInInt(Time) - (GetMinutes(Time) * 60);
}

int UTimerWidget::GetMinutes(const float Time) const{
	return GetTimerInInt(Time)/60;
}

void UTimerWidget::StartTimer(const float Timer, const FKOnFinishTimer FinishEvent, const bool RemoveTimerAtEnd){
	if(!IsVisible()){
		if(!MainPlayerController->IsWheelOpened()){
			AddToViewport();
		}
	}

	RemoveWidgetAtEnd = RemoveTimerAtEnd;
	CurrentDisplayTime = Timer;

	GetWorld()->GetTimerManager().SetTimer(DisplayTimer, this, &UTimerWidget::UpdateTimer, 0.1f, true);
	OnFinishTimer = FinishEvent;
}

void UTimerWidget::ChangeTimerValue(const float NewValue){
	const float LooseValue = CurrentDisplayTime - NewValue;

	CurrentDisplayTime = NewValue;

	FString SecondsValue;

	FString MinutesValue;

	GetTimerInString(LooseValue, MinutesValue, SecondsValue);

	LooseTimeText->SetVisibility(ESlateVisibility::Visible);
	LooseTimeText->SetText(FText::FromString(MinutesValue + ":" + SecondsValue));

	PlayAnimation(LooseTimeAnim, 0, 1, EUMGSequencePlayMode::Forward, LooseAnimSpeed);
}

bool UTimerWidget::IsTimerRunning() const{
	return GetWorld()->GetTimerManager().IsTimerActive(DisplayTimer);
}

float UTimerWidget::GetTimerElapsed() const{
	return CurrentDisplayTime;
}

void UTimerWidget::PauseTimer(const bool bPause){
	bPause ? GetWorld()->GetTimerManager().ClearTimer(DisplayTimer) : GetWorld()->GetTimerManager().SetTimer(DisplayTimer, this, &UTimerWidget::UpdateTimer, 0.1f, true);
}

void UTimerWidget::FinishTimer(const bool RemoveTimer){
	GetWorld()->GetTimerManager().ClearTimer(DisplayTimer);

	if(OnFinishTimer.IsBound()){
		OnFinishTimer.Execute();
	}

	if(RemoveTimer){
		RemoveWidget();
	}
}

void UTimerWidget::ForceFinishTimer(const bool ExecuteFinishedEvent, const bool RemoveTimer){
	if(!ExecuteFinishedEvent){
		OnFinishTimer.Clear();
	}

	FinishTimer(RemoveTimer);
}
