// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveButton.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "SaveAction.generated.h"

UCLASS(Abstract)
class GLITCHUE_API USaveAction : public UUserWidget{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* SaveButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* LoadButton;

public:
	void ResetBinds() const;

	void BindContextAction(USaveButton* SaveButtonReference) const;

	void UseSaveButton(const bool bUseSaveButton);
};
