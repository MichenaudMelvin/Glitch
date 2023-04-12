// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Player/MainPlayer.h"
#include "PlayerTriggerBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnPlayerEnter, AMainPlayer*, MainPlayer);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnPlayerExit, AMainPlayer*, MainPlayer);

UCLASS()
class GLITCHUE_API APlayerTriggerBox : public ATriggerBox{
	GENERATED_BODY()

public:
	APlayerTriggerBox();

protected:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FKOnPlayerEnter OnPlayerEnter;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FKOnPlayerExit OnPlayerExit;

	UFUNCTION()
	void EnterTriggerBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ExitTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
