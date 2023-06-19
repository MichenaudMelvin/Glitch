// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsefulStructs.generated.h"

USTRUCT(BlueprintType)
struct FTchatStruct {
	GENERATED_BODY()

public:
	FTchatStruct();

	FTchatStruct(FString NewSpeaker, FString NewTextMessage, FLinearColor NewSpeakerColor, float NewDelayForNextMessage);

	UPROPERTY(EditAnywhere, Category = "Tchat")
	FString Speaker;

	UPROPERTY(EditAnywhere, Category = "Tchat")
	FString TextMessage;

	UPROPERTY(EditAnywhere, Category = "Tchat")
	FLinearColor SpeakerColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tchat")
	float DelayForNextMessage = 0.5f;
};