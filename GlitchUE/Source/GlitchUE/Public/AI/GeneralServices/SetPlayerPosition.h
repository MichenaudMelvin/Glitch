// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SetPlayerPosition.generated.h"

UCLASS()
class GLITCHUE_API USetPlayerPosition : public UBTService{
	GENERATED_BODY()

public:
	USetPlayerPosition();

private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category = "Player")
	FBlackboardKeySelector Player;

	UPROPERTY(EditAnywhere, Category = "Player")
	FBlackboardKeySelector PlayerPosition;

};
