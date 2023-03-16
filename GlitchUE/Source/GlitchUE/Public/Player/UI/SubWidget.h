// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "SubWidget.generated.h"

UCLASS()
class GLITCHUE_API USubWidget : public UUserWidget{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	UUserWidget* OwnerWidget;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* BackButton;

public:
	virtual void AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder) override;

	UFUNCTION()
	void BackToOwnerWidget();

};
