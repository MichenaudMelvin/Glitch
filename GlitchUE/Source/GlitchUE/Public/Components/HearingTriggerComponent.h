// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HearingComponent.h"
#include "Components/ActorComponent.h"
#include "HearingTriggerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLITCHUE_API UHearingTriggerComponent : public UActorComponent{
	GENERATED_BODY()

public:
	UHearingTriggerComponent();

protected:
	virtual void BeginPlay() override;

	//bool bRefreshActorListAtNewSound = false;

	TArray<UHearingComponent*> HearingComponents;

public:
	void MakeNoise(AActor* Investigator, const FVector NoiseLocation, const float Radius);
};
