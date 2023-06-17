// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "UI/Custom/CustomButton.h"
#include "UI/Custom/CustomUserWidget.h"
#include "CreditsScreen.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UCreditsScreen : public UCustomUserWidget{
	GENERATED_BODY()

public:
	UCreditsScreen(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UBorder* FadeBorder;

	UPROPERTY(EditDefaultsOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeAnimation;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* EndMessage;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UCustomButton* GoToCreditButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UCustomButton* BackToMenuButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UCanvasPanel* MessageLayer;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UCanvasPanel* CreditLayer;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	bool bFadeIn = true;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	float FadeTime = 1;

	UFUNCTION()
	void SwitchToCredits();

	UFUNCTION()
	void BackToMainMenu();

	UPROPERTY()
	TSoftObjectPtr<UWorld> MainMenu;

public:
	void SetEndMessage(const FString NewEndMessage);
};
