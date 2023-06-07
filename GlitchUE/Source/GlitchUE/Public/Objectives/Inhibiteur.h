// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objectives/AbstractObjectif.h"
#include "Catalyseur.h"
#include "Components/CompassIcon.h"
#include "Inhibiteur.generated.h"

UCLASS()
class GLITCHUE_API AInhibiteur : public AAbstractObjectif{
	GENERATED_BODY()

public:
	AInhibiteur();

protected:
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	virtual void ActiveObjectif() override;

	virtual void OnSwitchPhases(EPhases CurrentPhase) override;

	void DestroyInhibteur();

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UPopcornFXEmitterComponent* LockerFX;

	UAnimationAsset* ActivationAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Compass")
	UCompassIcon* CompassIcon;

	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer) override;

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
