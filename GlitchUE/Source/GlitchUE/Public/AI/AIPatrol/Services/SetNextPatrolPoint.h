// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIPatrol/PatrolPoint.h"
#include "BehaviorTree/BTService.h"
#include "SetNextPatrolPoint.generated.h"

UCLASS()
class GLITCHUE_API USetNextPatrolPoint : public UBTService{
	GENERATED_BODY()

public:
	USetNextPatrolPoint();
	
private:
	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	TArray<APatrolPoint*> PatrolPointList;

	UPROPERTY(EditAnywhere, Category = "PatrolPoint")
	FBlackboardKeySelector PatrolPointKey;

	UPROPERTY(EditAnywhere, Category = "PatrolPoint")
	FBlackboardKeySelector CurrentPatrolPointIndex;
};
