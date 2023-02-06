// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Saves/AbstractSave.h"
#include "AI/MainAICharacter.h"
#include "Player/MainPlayer.h"
#include "WorldSave.generated.h"

UCLASS()
class GLITCHUE_API UWorldSave : public UAbstractSave{
	GENERATED_BODY()

public:
	UWorldSave();
	
protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<AMainAICharacter*> AIList;

	UPROPERTY(BlueprintReadWrite)
	AMainPlayer* Player;
};
