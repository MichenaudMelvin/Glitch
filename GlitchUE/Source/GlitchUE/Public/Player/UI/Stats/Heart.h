// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Heart.generated.h"

UCLASS()
class GLITCHUE_API UHeart : public UUserWidget{
	GENERATED_BODY()

public:
	UHeart(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Image", meta = (BindWidget))
	UImage* LeftImage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Image", meta = (BindWidget))
	UImage* RightImage;

	int Damages = 0;

	UTexture2D* FullHeart;

	UTexture2D* EmptyHeart;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateDamages(const bool TakeDamages, const int DMG) const;

	void SetImage(UImage* TargetImage, const bool bFullImage = true) const;
};
