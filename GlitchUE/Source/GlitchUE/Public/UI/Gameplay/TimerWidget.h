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
	UTextBlock* Minutes;

	UPROPERTY(EditDefaultsOnly, Category = "TimerText",  meta = (BindWidget))
	UTextBlock* Seconds;

	UPROPERTY(EditDefaultsOnly, Category = "Animations", Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Fade;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	float FadeTime = 2;

	FString SelectString(const int IntValue);

	float CurrentDisplayTime;

	void UpdateTimer();

	FTimerHandle DisplayTimer;

public:
	UFUNCTION(BlueprintCallable)
	void StartTimer(const float Timer, const FKOnFinishTimer FinishEvent);

	UFUNCTION(BlueprintCallable)
	void ChangeTimerValue(const float NewValue);

	UFUNCTION(BlueprintCallable)
	void FinishTimer();

	UFUNCTION(BlueprintCallable)
	void ForceFinishTimer(const bool ExecuteFinishedEvent = true);

	FKOnFinishTimer OnFinishTimer;

	bool IsTimerRunning() const;

	UFUNCTION(BlueprintCallable)
	float GetTimerElapsed() const;
};
