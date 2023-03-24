// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "StopBehavior.generated.h"

UCLASS()
class GLITCHUE_API UStopBehavior : public UBTTaskNode{
	GENERATED_BODY()

public:
	UStopBehavior();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
