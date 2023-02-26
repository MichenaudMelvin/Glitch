// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "SightIndication.generated.h"


UCLASS()
class GLITCHUE_API USightIndication : public UUserWidget{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	float TopSize;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UCanvasPanel* MainCanvas;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* Grey;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* Red;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateSightIndication(const float Value);
};
