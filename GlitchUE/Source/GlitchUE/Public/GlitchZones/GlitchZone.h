// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "GlitchZone.generated.h"

UCLASS()
class GLITCHUE_API AGlitchZone : public ATriggerBox{
	GENERATED_BODY()

public:
	AGlitchZone();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void EnterGlitchZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ExitGlitchZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
