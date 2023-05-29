// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Player/MainPlayer.h"
#include "UI/Custom/CustomButton.h"
#include "SelectionButton.generated.h"

UCLASS(Abstract)
class GLITCHUE_API USelectionButton : public UUserWidget, public IUIFocus{
	GENERATED_BODY()

protected:
	USelectionButton(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* Image;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Name;

	FLinearColor OriginalColor;

	AMainPlayer* MainPlayer;

	AMainPlayerController* MainPlayerController;

public:
	UFUNCTION()
	virtual void Select();

	UFUNCTION()
	virtual void UnSelect();

	virtual void BindButtons();

	virtual void UnbindButtons();

	virtual void ReceiveFocus() override;

	virtual void UnReceiveFocus() override;
};
