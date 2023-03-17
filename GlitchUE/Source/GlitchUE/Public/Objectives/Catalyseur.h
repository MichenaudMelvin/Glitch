// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inhibiteur.h"
#include "Objectives/AbstractObjectif.h"
#include "Nexus.h"
#include "Components/CompassIcon.h"
#include "PlacableObject/ConstructionZone.h"
#include "Catalyseur.generated.h"

USTRUCT(BlueprintType)
struct FStateAtWave{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EnableAtWave = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DisableAtWave = 999;
};

UCLASS()
class GLITCHUE_API ACatalyseur : public AAbstractObjectif{
	GENERATED_BODY()

public:
	ACatalyseur();

protected:
	virtual void BeginPlay() override;

	virtual void ActiveObjectif() override;

	virtual void DesactivateObjectif() override;

	virtual void HealthNull() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* TECHMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves", meta = (ExposeOnSpawn = "true"))
	FStateAtWave StateAtWave;

	ANexus* Nexus;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ConstructionZone", meta = (ExposeOnSpawn = "true"))
	TArray<AConstructionZone*> ConstructionZoneList;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inhibiteur", meta = (ExposeOnSpawn = "true"))
	TArray<AInhibiteur*> NearInhibiteur;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inhibiteur")
	UPaperSprite* InhibiteurSprite;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inhibiteur")
	float CompassRadius = 90;

	void GenerateCompass();

	void DeleteCompass();

	TArray<USceneComponent*> SceneComponents;

	TArray<UPaperSpriteComponent*> PaperSpriteComponents;

public:
	FStateAtWave GetStateAtWave() const;
};
