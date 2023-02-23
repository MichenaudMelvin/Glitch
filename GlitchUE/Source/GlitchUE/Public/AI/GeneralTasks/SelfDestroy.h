// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SelfDestroy.generated.h"

UCLASS()
class GLITCHUE_API USelfDestroy : public UBTTaskNode{
	GENERATED_BODY()

public:
	USelfDestroy();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
