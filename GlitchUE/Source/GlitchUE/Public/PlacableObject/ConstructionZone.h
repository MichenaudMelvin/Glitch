// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Components/ActivableComponent.h"
#include "PlacableActor.h"
#include "ConstructionZone.generated.h"

UCLASS()
class GLITCHUE_API AConstructionZone : public ATriggerBox{
	GENERATED_BODY()

public:
	AConstructionZone();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UActivableComponent* ActivableComp;

	UPROPERTY()
	UPopcornFXEmitterComponent* ConstructionFXEmitter;

	UPopcornFXEffect* ConstructionEffect;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EState InitialState;

	UFUNCTION()
	void ActiveObjectif();

	UFUNCTION()
	void DesactivateObjectif();

	APlacableActor* UnitInZone;

	UFUNCTION(BlueprintCallable)
	void OccupiedSlot(APlacableActor* NewUnit);

public:
	UFUNCTION(BlueprintCallable)
	void UnoccupiedSlot();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Activable")
	UActivableComponent* GetActivableComp();
};
