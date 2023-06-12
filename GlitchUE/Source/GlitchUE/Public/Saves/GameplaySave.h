// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Saves/AbstractSave.h"
#include "GameplaySave.generated.h"

UCLASS()
class GLITCHUE_API UGameplaySave : public UAbstractSave{
	GENERATED_BODY()

public:
	UGameplaySave();

	UPROPERTY(BlueprintReadWrite)
	FVector2D LastedMousePosition;

	UPROPERTY(BlueprintReadWrite)
	FKey LastPressedKey = EKeys::LeftMouseButton;

	bool IsUsingGamepad() const;
};
