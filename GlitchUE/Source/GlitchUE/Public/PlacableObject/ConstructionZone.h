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

	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", meta = (ExposeOnSpawn = "true"))
	EState InitialState;

	void ToggleActivation(bool bActivate);

	void ActiveObjectif();

	void DesactivateObjectif();

	APlacableActor* UnitInZone;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OccupiedSlot(APlacableActor* NewUnit);
	virtual void OccupiedSlot_Implementation(APlacableActor* NewUnit);

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UnoccupiedSlot();
	virtual void UnoccupiedSlot_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Activable")
	UActivableComponent* GetActivableComp();
};
