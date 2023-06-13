// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Custom/CustomButton.h"
#include "UI/Custom/CustomUserWidget.h"
#include "ConditionScreen.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UConditionScreen : public UCustomUserWidget{
	GENERATED_BODY()

public:
	UConditionScreen(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "Buttons", meta = (BindWidget))
	UCustomButton* BackButton;

	UFUNCTION()
	void BackToMainMenu();

	UPROPERTY()
	TSoftObjectPtr<UWorld> MainMenu;
};
