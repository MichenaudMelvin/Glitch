// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UI/UIFocus.h"
#include "CustomButton.generated.h"

UCLASS()
class GLITCHUE_API UCustomButton : public UButton, public IUIFocus{
	GENERATED_BODY()

protected:
	virtual void SynchronizeProperties() override;

	FButtonStyle SavedStyle;

public:
	virtual void ReceiveFocus() override;

	virtual void UnReceiveFocus() override;
};
