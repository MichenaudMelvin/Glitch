// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "LaunchPursuitDrone.generated.h"

UCLASS()
class GLITCHUE_API ULaunchPursuitDrone : public UBTTaskNode{
	GENERATED_BODY()

	ULaunchPursuitDrone();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void FinishAnims();

	UPROPERTY()
	UBehaviorTreeComponent* OwnerBehaviorTree;

	FTimerDelegate AnimsTimer;

	FTimerHandle TimerHandle;

	// Set as false will reverse animations
	UPROPERTY(EditAnywhere, Category = "Behavior")
	bool bLaunch = true;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	bool bSkipAnim = false;
};
