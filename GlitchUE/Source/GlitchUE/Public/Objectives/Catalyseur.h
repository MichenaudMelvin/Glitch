// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objectives/AbstractObjectif.h"
#include "Nexus.h"
#include "Catalyseur.generated.h"

UCLASS()
class GLITCHUE_API ACatalyseur : public AAbstractObjectif{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void ActiveObjectif() override;

	virtual void DesactivateObjectif() override;

	virtual void OnHealthNull() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position", meta = (ExposeOnSpawn = "true"))
	ANexus* Nexus;
};
