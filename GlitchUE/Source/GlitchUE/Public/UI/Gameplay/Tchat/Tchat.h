// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TchatLine.h"
#include "Components/ListView.h"
#include "Helpers/UsefulStructs.h"
#include "UI/Custom/CustomUserWidget.h"
#include "Tchat.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnFinishWritingMessageList);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnTchatExtendStart, bool, Forward);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnTchatExtendEnd, bool, Forward);

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

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Tchat")
	UTextBlock* InstructionText;

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

	UPROPERTY(EditDefaultsOnly, Category = "Instruction")
	FString GlobalInstruction = "To open or close the tchat press";

	UPROPERTY(EditDefaultsOnly, Category = "Instruction")
	FString KeyboardButton = "T";

	UPROPERTY(EditDefaultsOnly, Category = "Instruction")
	FString GamepadButton = "LB";

	UFUNCTION()
	void SetKeyboardInstruction();

	UFUNCTION()
	void SetGamepadInstruction();

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates")
	FKOnFinishWritingMessageList OnFinishWritingMessageList;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates")
	FKOnTchatExtendStart OnTchatExtendStart;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates")
	FKOnTchatExtendEnd OnTchatExtendEnd;

	UFUNCTION(BlueprintCallable)
	void OpenTchat();

	UFUNCTION(BlueprintCallable)
	void CloseTchat();

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<FTchatStruct> AllTchatLines;

	UPROPERTY(BlueprintReadWrite)
	int TargetScrollIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tchat|Scroll")
	int ScrollMultiplier = 1;

	UPROPERTY(BlueprintReadOnly, Category = "Tchat")
	TArray<FTchatStruct> CurrentListToAdd;

	bool bWritingMultipleMessages = false;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category  = "Tchat")
	void WriteMessageList();
	virtual void WriteMessageList_Implementation();

	UFUNCTION(BlueprintCallable)
	void GamepadScroll(float Axis);

	UFUNCTION(BlueprintCallable)
	void MouseScroll(float Axis);

	bool bRequestManualScroll = false;

public:
	UFUNCTION(BlueprintCallable, Category = "Tchat")
	void AddTchatLine(const FString NewSpeaker, const FString NewMessage, const FLinearColor SpeakerColor);

	UFUNCTION(BlueprintCallable, Category = "Tchat")
	void AddEmptyTchatLine();

	UFUNCTION(BlueprintCallable, Category = "Tchat")
	void AddMultipleTchatLines(TArray<FTchatStruct> TchatLines);

	bool IsOpenByUser() const;

	TArray<FTchatStruct> GetAllTchatLines() const;

	UFUNCTION(BlueprintCallable, Category = "Tchat")
	void AddTchatLineWithATchatStruct(FTchatStruct TchatStruct);
};
