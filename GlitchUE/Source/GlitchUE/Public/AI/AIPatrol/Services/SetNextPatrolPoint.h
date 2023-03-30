// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SetNextPatrolPoint.generated.h"

UCLASS()
class GLITCHUE_API USetNextPatrolPoint : public UBTService{
	GENERATED_BODY()

public:
	USetNextPatrolPoint();

private:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "PatrolPoint")
	FBlackboardKeySelector PatrolPointKey;

	UPROPERTY(EditAnywhere, Category = "PatrolPoint")
	FBlackboardKeySelector CurrentPatrolPointIndex;
};
