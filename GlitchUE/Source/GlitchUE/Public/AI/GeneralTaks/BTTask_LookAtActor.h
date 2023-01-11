// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_LookAtActor.generated.h"

class AAIController;

UCLASS()
class GLITCHUE_API UBTTask_LookAtActor : public UBTTaskNode{
	GENERATED_BODY()

protected:
	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector Target;

public:
	UBTTask_LookAtActor();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
