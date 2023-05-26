// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Player/MainPlayer.h"
#include "SelectionButton.generated.h"

UCLASS(Abstract)
class GLITCHUE_API USelectionButton : public UUserWidget{
	GENERATED_BODY()

protected:
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
};
