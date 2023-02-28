// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "StunAI.generated.h"

UCLASS()
class GLITCHUE_API UStunAI : public UBTTaskNode{
	GENERATED_BODY()

public:
	UStunAI();

private:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Stun")
	FBlackboardKeySelector StunTime;

	float RemainingWaitTime;
};
