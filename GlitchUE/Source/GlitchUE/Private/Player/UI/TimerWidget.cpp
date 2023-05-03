// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/TimerWidget.h"

void UTimerWidget::NativeConstruct(){
	Super::NativeConstruct();

	PlayAnimation(Fade, 0, 1, EUMGSequencePlayMode::Forward, FadeTime);
}

void UTimerWidget::RemoveWidget(){
	PlayAnimation(Fade, 0, 1, EUMGSequencePlayMode::Reverse, FadeTime);

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){
		RemoveFromParent();
	}, FadeTime, false);
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

	const int IntTimerValue = FMath::RoundToInt(CurrentDisplayTime);

	const int MinutesInt = IntTimerValue/60;

	const int SecondsInt = IntTimerValue - (MinutesInt * 60);

	const FString SecondsValue = SelectString(SecondsInt);

	const FString MinutesValue = SelectString(MinutesInt);

	Seconds->SetText(FText::FromString(SecondsValue));
	Minutes->SetText(FText::FromString(MinutesValue));

	if(MinutesInt <= 0 && SecondsInt <= 0){
		FinishTimer();
	}
}

void UTimerWidget::StartTimer(const float Timer, const FKOnFinishTimer FinishEvent){
	AddToViewport();

	CurrentDisplayTime = Timer;

	GetWorld()->GetTimerManager().SetTimer(DisplayTimer, this, &UTimerWidget::UpdateTimer, 0.1f, true);
	OnFinishTimer = FinishEvent;
}

bool UTimerWidget::IsTimerRunning() const{
	return GetWorld()->GetTimerManager().IsTimerActive(DisplayTimer);
}

void UTimerWidget::FinishTimer(){
	GetWorld()->GetTimerManager().ClearTimer(DisplayTimer);

	if(OnFinishTimer.IsBound()){
		OnFinishTimer.Execute();
	}

	RemoveWidget();
}

void UTimerWidget::ForceFinishTimer(const bool ExecuteFinishedEvent){
	if(ExecuteFinishedEvent){
		OnFinishTimer.Clear();
	}

	FinishTimer();
}
