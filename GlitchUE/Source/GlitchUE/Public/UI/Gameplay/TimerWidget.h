// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "TimerWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE(FKOnFinishTimer);

UCLASS(Abstract)
class GLITCHUE_API UTimerWidget : public UUserWidget{
	GENERATED_BODY()

protected:
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

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	float LooseAnimSpeed = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	float FadeTime = 2;

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
	UFUNCTION(BlueprintCallable)
	void StartTimer(const float Timer, const FKOnFinishTimer FinishEvent, const bool RemoveTimerAtEnd = true);

	UFUNCTION(BlueprintCallable)
	void ChangeTimerValue(const float NewValue);

	UFUNCTION(BlueprintCallable)
	void FinishTimer(const bool RemoveTimer);

	UFUNCTION(BlueprintCallable)
	void ForceFinishTimer(const bool ExecuteFinishedEvent = true, const bool RemoveTimer = true);

	FKOnFinishTimer OnFinishTimer;

	bool IsTimerRunning() const;

	UFUNCTION(BlueprintCallable)
	float GetTimerElapsed() const;
};
