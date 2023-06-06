// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CheckBox.h"
#include "UI/UIFocus.h"
#include "CustomCheckbox.generated.h"

UCLASS()
class GLITCHUE_API UCustomCheckbox : public UCheckBox, public IUIFocus{
	GENERATED_BODY()

public:
	UCustomCheckbox();

protected:
	virtual void SynchronizeProperties() override;

	FCheckBoxStyle SavedStyle;

	float CheckBoxSize = 92.0f;

public:
	virtual void ReceiveFocus() override;

	virtual void UnReceiveFocus() override;
};
