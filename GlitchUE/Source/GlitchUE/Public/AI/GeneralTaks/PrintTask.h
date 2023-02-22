// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PrintTask.generated.h"

UCLASS()
class GLITCHUE_API UPrintTask : public UBTTaskNode{
	GENERATED_BODY()

public:
	UPrintTask();

private:
	UPROPERTY(EditAnywhere, Category = "Print")
	FBlackboardKeySelector VariableToPrint;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
