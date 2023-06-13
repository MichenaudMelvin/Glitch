// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Helpers/UsefulStructs.h"
#include "TchatTriggerBox.generated.h"

class AMainPlayerController;

UCLASS()
class GLITCHUE_API ATchatTriggerBox : public ATriggerBox{
	GENERATED_BODY()

public:
	ATchatTriggerBox();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Tchat")
	TArray<FTchatStruct> TchatMessageList;

	AMainPlayerController* CurrentController;

	int Index = 0;

	UFUNCTION()
	void EnterTriggerBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void WriteMessages();
};
