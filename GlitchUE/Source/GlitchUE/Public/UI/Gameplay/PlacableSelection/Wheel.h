// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObjectButton.h"
#include "UI/Custom/CustomButton.h"
#include "UI/Custom/CustomUserWidget.h"
#include "Wheel.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UWheel : public UCustomUserWidget{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	virtual void FocusWidgets() override;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UCustomButton* DestructButton;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UTextBlock* PlacableDescription;

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
	TArray<UPlacableObjectButton*> ButtonList;

	UPROPERTY(BlueprintReadOnly)
	AMainPlayer* MainPlayer;

	UFUNCTION()
	void ClickOnDestructButton();

public:

	void ClickOnDestructButtonDelay();

	void SetDescription(const FText NewDescription) const;
};
