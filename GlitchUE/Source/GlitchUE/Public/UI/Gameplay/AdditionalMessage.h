// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "AdditionalMessage.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UAdditionalMessage : public UUserWidget{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AdditionalMessage",  meta = (BindWidget))
	UTextBlock* AdditionalMessageText;

	UPROPERTY(EditDefaultsOnly, Category = "Animations", Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* AdditionalMessageAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	float AdditionalMessageAnimSpeed = 0.5f;

	FTimerHandle AnimTimerHandle;

public:
	void AddMessageToScreen(const FString NewMessage);
};
