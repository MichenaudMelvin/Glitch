// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AlertNearestAI.generated.h"

UCLASS()
class GLITCHUE_API UAlertNearestAI : public UBTTaskNode{
	GENERATED_BODY()

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	TArray<AActor*> AIList;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	int NumberOfAIToCall = 5;
};
