// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SwitchBehavior.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnSwitchBehavior);

UCLASS()
class GLITCHUE_API USwitchBehavior : public UBTTaskNode{
	GENERATED_BODY()

public:
	USwitchBehavior();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	UBehaviorTree* NewBehaviorTree;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	UBlackboardData* NewBlackboardData;
};
