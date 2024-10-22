// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomButton.h"
#include "CustomUserWidget.h"
#include "SubWidget.generated.h"

UCLASS(Abstract)
class GLITCHUE_API USubWidget : public UCustomUserWidget{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	UUserWidget* OwnerWidget;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UCustomButton* BackButton;

public:
	virtual void AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder) override;

	UFUNCTION()
	void BackToOwnerWidget();

	void SetOwnerWidget(UUserWidget* NewOwnerWidget);
};
