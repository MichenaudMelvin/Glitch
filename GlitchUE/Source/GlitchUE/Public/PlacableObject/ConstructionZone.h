// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Components/ActivableComponent.h"
#include "ConstructionZone.generated.h"

UCLASS()
class GLITCHUE_API AConstructionZone : public ATriggerBox{
	GENERATED_BODY()

public:
	AConstructionZone();

protected:

	UActivableComponent* ActivableComp;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Activable")
	UActivableComponent* GetActivableComp();
};
