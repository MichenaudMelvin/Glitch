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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UActivableComponent* ActivableComp;

	UPopcornFXEmitterComponent* ConstructionFXEmitter;

	UPopcornFXEffect* ConstructionEffect;

	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);

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
