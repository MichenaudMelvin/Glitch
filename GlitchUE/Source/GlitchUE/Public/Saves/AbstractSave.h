// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AbstractSave.generated.h"

UCLASS()
class GLITCHUE_API UAbstractSave : public USaveGame{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Name")
	FString SlotName;

public:
	FString GetSlotName();
};
