// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/MainAICharacter.h"
#include "PursuitDrone.generated.h"

UCLASS()
class GLITCHUE_API APursuitDrone : public AMainAICharacter{
	GENERATED_BODY()

public:
	APursuitDrone();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UInteractableComponent* InteractableComp;

	UFUNCTION()
	void OnTouchSomething(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UFUNCTION()
	void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer);

	UInteractableComponent* GetInteractableComp() const;
};
