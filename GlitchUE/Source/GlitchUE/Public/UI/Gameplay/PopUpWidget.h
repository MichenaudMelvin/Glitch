// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "UI/Custom/CustomButton.h"
#include "UI/Custom/CustomUserWidget.h"
#include "PopUpWidget.generated.h"

class AMainPlayerController;

UCLASS(Abstract)
class GLITCHUE_API UPopUpWidget : public UCustomUserWidget{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeDestruct() override;

	AMainPlayerController* MainPlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "Buttons", meta = (BindWidget))
	UTextBlock* PopUpText;

	UPROPERTY(EditDefaultsOnly, Category = "Buttons", meta = (BindWidget))
	UCustomButton* BackButton;

	TArray<FString> CurrentPopUpArray;

	int ArrayCount = 0;

	void InitWidget();

	UFUNCTION()
	void GoToNextPopUp();

	bool bRebindAtDestruct = false;

public:
	UFUNCTION(BlueprintCallable, Category = "PopUp")
	void ShowPopUp(const FString PopUpMessage, const bool bRebindAtEnd = false);

	UFUNCTION(BlueprintCallable, Category = "PopUp")
	void ShowMultiplePopUps(const TArray<FString> PopUpMessages, const bool bRebindAtEnd = false);
};
