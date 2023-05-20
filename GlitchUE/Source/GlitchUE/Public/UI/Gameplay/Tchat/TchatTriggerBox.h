// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "TchatTriggerBox.generated.h"

class AMainPlayerController;

USTRUCT(BlueprintType)
struct FTchatStruct {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Tchat")
	FString Speaker;

	UPROPERTY(EditAnywhere, Category = "Tchat")
	FString TextMessage;

	UPROPERTY(EditAnywhere, Category = "Tchat")
	FLinearColor SpeakerColor = FLinearColor(0, 0, 0, 1);
};

UCLASS()
class GLITCHUE_API ATchatTriggerBox : public ATriggerBox{
	GENERATED_BODY()

public:
	ATchatTriggerBox();

protected:
	UPROPERTY(EditAnywhere, Category = "Tchat")
	TArray<FTchatStruct> TchatMessageList;

	UPROPERTY(EditAnywhere, Category = "Tchat")
	float DelayBetweenEachMessage = 0.5f;

	AMainPlayerController* CurrentController;

	int Index = 0;

	UFUNCTION()
	void EnterTriggerBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void WriteMessages();
};
