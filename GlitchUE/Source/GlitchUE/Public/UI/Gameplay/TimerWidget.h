// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "TimerWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE(FKOnFinishTimer);

class AMainPlayerController;

UCLASS(Abstract)
class GLITCHUE_API UTimerWidget : public UUserWidget{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	void RemoveWidget();

	UPROPERTY(EditDefaultsOnly, Category = "TimerText",  meta = (BindWidget))
	UTextBlock* TimerText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations", Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Fade;

	UPROPERTY(EditDefaultsOnly, Category = "LooseTimeText",  meta = (BindWidget))
	UTextBlock* LooseTimeText;

	UPROPERTY(EditDefaultsOnly, Category = "Animations", Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* LooseTimeAnim;

	UPROPERTY()
	AMainPlayerController* MainPlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	float LooseAnimSpeed = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	float FadeTime = 2;

	bool bIsTimerRunning = false;

	FString SelectString(const int IntValue);

	float CurrentDisplayTime;

	bool RemoveWidgetAtEnd;

	void UpdateTimer();

	FTimerHandle DisplayTimer;

	void GetTimerInString(const float Timer, FString& Minutes, FString& Seconds);

	int GetTimerInInt(const float Time) const;

	int GetSeconds(const float Time) const;

	int GetMinutes(const float Time) const;

public:
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void StartTimer(const float Timer, const FKOnFinishTimer FinishEvent, const bool RemoveTimerAtEnd = true);

	/**
	 * @brief Remove time from time 
	 * @param NewValue the removed time
	 */
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void ChangeTimerValue(const float NewValue);

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void FinishTimer(const bool RemoveTimer);

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void ForceFinishTimer(const bool ExecuteFinishedEvent = true, const bool RemoveTimer = true);

	FKOnFinishTimer OnFinishTimer;

	UFUNCTION(BlueprintCallable, Category = "Timer")
	bool IsTimerRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Timer")
	float GetTimerElapsed() const;

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void PauseTimer(const bool bPause);
};
