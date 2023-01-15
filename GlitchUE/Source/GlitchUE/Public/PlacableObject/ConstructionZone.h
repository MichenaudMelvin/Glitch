// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Components/ActivableComponent.h"
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

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", meta = (ExposeOnSpawn = "true"))
	EState InitialState;

	void ToggleActivation(bool bActivate);

	void ActiveObjectif();

	void DesactivateObjectif();

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Activable")
	UActivableComponent* GetActivableComp();
};
