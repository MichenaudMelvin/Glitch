// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TakeMaxDamages.generated.h"

UCLASS()
class GLITCHUE_API UTakeMaxDamages : public UBTTaskNode{
	GENERATED_BODY()

public:
	UTakeMaxDamages();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
