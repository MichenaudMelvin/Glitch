// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TchatLineData.generated.h"

UCLASS()
class GLITCHUE_API UTchatLineData : public UObject{
	GENERATED_BODY()

public:
	FString Speaker;

	FString Message;

	FSlateColor SpeakerColor;
};
