// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PopUpWidget.generated.h"

class AMainPlayerController;

UCLASS(Abstract)
class GLITCHUE_API UPopUpWidget : public UUserWidget{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeDestruct() override;

	AMainPlayerController* MainPlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "Buttons", meta = (BindWidget))
	UTextBlock* PopUpText;

	UPROPERTY(EditDefaultsOnly, Category = "Buttons", meta = (BindWidget))
	UButton* BackButton;

	TArray<FString> CurrentPopUpArray;

	int ArrayCount = 0;

	void InitWidget();

	UFUNCTION()
	void GoToNextPopUp();

public:
	UFUNCTION(BlueprintCallable)
	void ShowPopUp(const FString PopUpMessage);

	UFUNCTION(BlueprintCallable)
	void ShowMultiplePopUps(const TArray<FString> PopUpMessages);
};
