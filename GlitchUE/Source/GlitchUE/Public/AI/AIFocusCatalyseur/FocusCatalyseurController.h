// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/FocusAIController.h"
#include "Objectives/Catalyseur.h"
#include "FocusCatalyseurController.generated.h"

UCLASS()
class GLITCHUE_API AFocusCatalyseurController : public AFocusAIController{
	GENERATED_BODY()

public:
	AFocusCatalyseurController();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	TArray<ACatalyseur*> CatalyseurList;

public:
	const TArray<ACatalyseur*> GetCatalyseurList();
};
