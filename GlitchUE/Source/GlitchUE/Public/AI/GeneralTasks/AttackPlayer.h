// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackTarget.h"
#include "AttackPlayer.generated.h"

UCLASS()
class GLITCHUE_API UAttackPlayer : public UAttackTarget{
	GENERATED_BODY()

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
