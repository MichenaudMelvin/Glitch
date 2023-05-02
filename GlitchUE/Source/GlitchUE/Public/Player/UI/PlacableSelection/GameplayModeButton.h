// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/MainPlayerController.h"
#include "Player/UI/PlacableSelection/SelectionButton.h"
#include "GameplayModeButton.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UGameplayModeButton : public USelectionButton{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(EditAnywhere)
	EGameplayMode GameplayMode;

public:
	virtual void Select() override;
};
