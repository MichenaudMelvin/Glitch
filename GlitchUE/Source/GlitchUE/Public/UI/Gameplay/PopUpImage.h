// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Custom/CustomButton.h"
#include "UI/Custom/CustomUserWidget.h"
#include "PopUpImage.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UPopUpImage : public UCustomUserWidget{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "Buttons", meta = (BindWidget))
	UCustomButton* BackButton;
};
