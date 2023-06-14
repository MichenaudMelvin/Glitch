// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UI/UIFocus.h"
#include "CustomButton.generated.h"

UCLASS()
class GLITCHUE_API UCustomButton : public UButton, public IUIFocus{
	GENERATED_BODY()

public:
	UCustomButton();

protected:
	virtual void SynchronizeProperties() override;

	FButtonStyle NormalStyle;

	UPROPERTY(EditAnywhere, Category = "Appearance");
	FSlateColor BlockedColor = FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f));

	bool bIsBlocked = false;

public:
	UFUNCTION(BlueprintCallable, Category = "Button")
	virtual void BlockButton(const bool bUnbindHovering);

	UFUNCTION(BlueprintCallable, Category = "Button")
	virtual void UnblockButton();

	UFUNCTION(BlueprintCallable, Category = "Button")
	bool IsBlocked() const;

	virtual void ReceiveFocus() override;

	virtual void UnReceiveFocus() override;
};
