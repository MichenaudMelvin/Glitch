// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objectives/AbstractObjectif.h"
#include "PlacableObject/ConstructionZone.h"
#include "Catalyseur.h"
#include "Inhibiteur.generated.h"

UCLASS()
class GLITCHUE_API AInhibiteur : public AAbstractObjectif{
	GENERATED_BODY()
public:
	AInhibiteur();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Construction", meta = (ExposeOnSpawn = "true"))
	TArray<AConstructionZone*> ConstructionZoneList;

	virtual void BeginPlay() override;

	virtual void ActiveObjectif() override;

	UAnimationAsset* ActivationAnim;

	FCompassSprite SpriteReference;

	virtual void DesactivateObjectif() override;

	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer) override;

	void ActivateLinkedElements(const bool bActivate);

public:
	void SetSpriteReference(const FCompassSprite NewSprite);

#if WITH_EDITORONLY_DATA
	void OnObjectSelected(UObject* Object);
#endif
};
