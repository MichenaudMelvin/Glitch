// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TchatLine.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "Tchat.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UTchat : public UUserWidget{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim), Category = "Animation")
	UWidgetAnimation* AppearAnimation;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Tchat")
	UListView* TchatList;

	TArray<UTchatLine*> TchatLines;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	float AppearanceDuration = 2;

	FTimerHandle DestructTimer;

	FTimerHandle DisappearTimer;

	FString LastSpeaker;

	void CheckDisappearance();

	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	float DestructTime = 3;

	bool IsOpenByUser;

	void StartDestructTimer();

	void StopDestructTimer();

	void ResetDestructTimer();

	UFUNCTION(BlueprintCallable)
	void OpenTchat();

	UFUNCTION(BlueprintCallable)
	void CloseTchat();

	void AddTchatLineDelay();

public:
	UFUNCTION(BlueprintCallable, Category = "Tchat")
	void AddTchatLine(const FString NewSpeaker, const FString NewMessage, const FLinearColor SpeakerColor);
};
