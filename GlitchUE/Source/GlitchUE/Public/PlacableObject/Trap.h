// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/PlacableActor.h"
#include "Components/ActivableComponent.h"
#include "PlacableObject/TrapData.h"
#include "Trap.generated.h"

UCLASS()
class GLITCHUE_API ATrap : public APlacableActor{
	GENERATED_BODY()

public:
	ATrap();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Activable")
	UActivableComponent* ActivableComp;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
	float Damages;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
	float TrapDuration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
	float TrapAttackRate;

	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer) override;

	virtual void GlitchUpgrade() override;

	virtual void SetData(UPlacableActorData* NewData) override;

};
