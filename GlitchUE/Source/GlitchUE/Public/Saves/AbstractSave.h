// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AbstractSave.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UAbstractSave : public USaveGame{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Save")
	FString SlotName;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Save")
	int Index;

	FString GetSlotName() const;

	int GetIndex() const;
};
