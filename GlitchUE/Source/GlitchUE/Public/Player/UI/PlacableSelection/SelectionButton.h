// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/MainPlayer.h"
#include "SelectionButton.generated.h"

UCLASS(Abstract)
class GLITCHUE_API USelectionButton : public UUserWidget{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeDestruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* Image;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Name;

	UPROPERTY(EditAnywhere)
	bool bAllowHover = true;

	UPROPERTY(EditAnywhere)
	bool bApplyWhenSelected = true;

	bool bIsSelectd = false;

	FLinearColor OriginalColor;

	AMainPlayer* MainPlayer;

	AMainPlayerController* MainPlayerController;

public:
	UFUNCTION()
	virtual void Select();

	UFUNCTION()
	virtual void UnSelect();
};
