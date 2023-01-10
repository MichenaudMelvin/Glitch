// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objectives/AbstractObjectif.h"
#include "PlacableObject/ConstructionZone.h"
#include "Components/InteractableComponent.h"
#include "Inhibiteur.generated.h"

UCLASS()
class GLITCHUE_API AInhibiteur : public AAbstractObjectif{
	GENERATED_BODY()
public:
	AInhibiteur();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Construction", meta = (ExposeOnSpawn = "true"))
	TArray<AConstructionZone*> ConstructionZoneList;
};
