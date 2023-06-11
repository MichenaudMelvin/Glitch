// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TchatLine.h"
#include "Components/ListView.h"
#include "Helpers/UsefulStructs.h"
#include "UI/Custom/CustomUserWidget.h"
#include "Tchat.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UTchat : public UCustomUserWidget{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim), Category = "Animation")
	UWidgetAnimation* AppearAnimation;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim), Category = "Animation")
	UWidgetAnimation* ExtendTchatAnim;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Tchat")
	UListView* TchatList;

	UPROPERTY()
	UTchatLine* LastTchatLine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	float AppearanceDuration = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	float ExtendDuration = 2;

	FTimerHandle DestructTimer;

	FTimerHandle DisappearTimer;

	FString LastSpeaker;

	void CheckDisappearance();

	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	float DestructTime = 3;

	bool bIsOpenByUser;

	void StartDestructTimer();

	void StopDestructTimer();

	void ResetDestructTimer();

public:
	UFUNCTION(BlueprintCallable)
	void OpenTchat();

	UFUNCTION(BlueprintCallable)
	void CloseTchat();

protected:
	void AddTchatLineDelay();

	void RebuildList() const;

	UObject* LastItem;

	FString CurrentSpeaker;

	FString CurrentMessage;

	FLinearColor CurrentSpeakerColor;

	UPROPERTY()
	TArray<FTchatStruct> AllTchatLines;

public:
	UFUNCTION(BlueprintCallable, Category = "Tchat")
	void AddTchatLine(const FString NewSpeaker, const FString NewMessage, const FLinearColor SpeakerColor);

	bool IsOpenByUser() const;

	TArray<FTchatStruct> GetAllTchatLines() const;

	UFUNCTION(BlueprintCallable, Category = "Tchat")
	void AddTchatLineWithATchatStruct(FTchatStruct TchatStruct);
};
