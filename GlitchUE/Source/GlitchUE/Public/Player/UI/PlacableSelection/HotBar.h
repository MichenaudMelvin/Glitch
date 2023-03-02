// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/UI/PlacableSelection/PlacableSelection.h"
#include "HotBar.generated.h"

UCLASS()
class GLITCHUE_API UHotBar : public UPlacableSelection{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeIn;

	int CurrentIndex = 0;

	UFUNCTION(BlueprintCallable)
	void Scroll(const float Value);

	UFUNCTION()
	void SelectIndex();

	UFUNCTION(BlueprintCallable)
	void PlayFadeOutAnimation();
};
