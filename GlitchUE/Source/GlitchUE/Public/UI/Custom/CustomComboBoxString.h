// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ComboBoxString.h"
#include "UI/UIFocus.h"
#include "CustomComboBoxString.generated.h"

UCLASS()
class GLITCHUE_API UCustomComboBoxString : public UComboBoxString, public IUIFocus{
	GENERATED_BODY()

public:
	UCustomComboBoxString();

protected:
	virtual void SynchronizeProperties() override;

	FButtonStyle NormalStyle;

public:
	virtual void ReceiveFocus() override;

	virtual void UnReceiveFocus() override;
};
