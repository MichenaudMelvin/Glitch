// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "FadeScreen.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UFadeScreen : public UUserWidget{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UBorder* FadeBorder;

	UPROPERTY(EditDefaultsOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeAnimation;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	bool bFadeIn = true;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	float FadeTime = 1;
};
