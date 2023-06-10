// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "UI/Custom/CustomButton.h"
#include "Player/MainPlayer.h"
#include "PlacableObjectButton.generated.h"

class UWheel;

UCLASS()
class GLITCHUE_API UPlacableObjectButton : public UCustomButton{
	GENERATED_BODY()

public:
	UPlacableObjectButton();

protected:
	virtual void SynchronizeProperties() override;

	UPROPERTY(EditAnywhere, Category = "Data")
	UPlacableActorData* Data;

	UPROPERTY()
	UTextBlock* Name;

	UWheel* Wheel;

	AMainPlayer* MainPlayer;

	AMainPlayerController* MainPlayerController;

	UFUNCTION()
	void OnClick();

public:
	UFUNCTION()
	virtual void Select();

	UFUNCTION()
	virtual void UnSelect();

	virtual void BindButtons();

	virtual void UnbindButtons();

	bool CompareData(const UPlacableActorData* DataToCompare) const;

	void SetWheel(UWheel* NewWheel);

	virtual void ReceiveFocus() override;

	virtual void UnReceiveFocus() override;
};
