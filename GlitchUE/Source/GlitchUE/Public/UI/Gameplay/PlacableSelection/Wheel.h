// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObjectButton.h"
#include "Components/CanvasPanel.h"
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

	UPROPERTY()
	AMainPlayer* MainPlayer;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* GoldsText;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UCanvasPanel* DescriptionPanel;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* DescriptionText;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* DamagesText;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* AttackSpeedText;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* RangeText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* WallText;

	UFUNCTION()
	void ClickOnDestructButton();

public:
	UFUNCTION()
	void UpdateDisplayGolds(int Golds);

	void ClickOnDestructButtonDelay();

	UFUNCTION(BlueprintNativeEvent)
	void SetDescription(const UPlacableActorData* Data) const;
	virtual void SetDescription_Implementation(const UPlacableActorData* Data) const;
};
