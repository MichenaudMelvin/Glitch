// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objectives/AbstractObjectif.h"
#include "Nexus.generated.h"

class AWaveManager;

UCLASS()
class GLITCHUE_API ANexus : public AAbstractObjectif{
	GENERATED_BODY()

public:
	ANexus();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves", meta = (ExposeOnSpawn = "true"))
	AWaveManager* WaveManager;

	virtual void ActiveObjectif() override;
	
	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer) override;
};