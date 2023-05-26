// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObjectButton.h"
#include "SubMenuButton.h"
#include "Blueprint/UserWidget.h"
#include "PlacableSelection.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UPlacableSelection : public UUserWidget{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPlacableObjectButton* PlacableButton1;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPlacableObjectButton* PlacableButton2;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPlacableObjectButton* PlacableButton3;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPlacableObjectButton* PlacableButton4;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPlacableObjectButton* PlacableButton5;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPlacableObjectButton* PlacableButton6;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPlacableObjectButton* PlacableButton7;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPlacableObjectButton* PlacableButton8;

	UPROPERTY(BlueprintReadOnly)
	TArray<USelectionButton*> ButtonList; 

	UPROPERTY(BlueprintReadOnly)
	AMainPlayerController* MainPlayerController;

	UPROPERTY(BlueprintReadOnly)
	AMainPlayer* MainPlayer;
};