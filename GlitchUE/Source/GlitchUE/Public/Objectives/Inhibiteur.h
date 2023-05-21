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

	virtual void Destroyed() override;

	virtual void ActiveObjectif() override;

	UAnimationAsset* ActivationAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Compass")
	UCompassIcon* CompassIcon;

	virtual void DesactivateObjectif() override;

	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer) override;

	void ActivateLinkedElements(const bool bActivate);

	UPROPERTY(BlueprintReadOnly)
	ACatalyseur* OwnerCatalyseur;

public:
	void SetOwnerCatalyseur(ACatalyseur* NewOwner);

#if WITH_EDITORONLY_DATA
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void OnObjectSelected(UObject* Object);

	void OutlineLinkedObjects(const bool bOutline);

	virtual void PreSave(const ITargetPlatform* TargetPlatform) override;
#endif
};
