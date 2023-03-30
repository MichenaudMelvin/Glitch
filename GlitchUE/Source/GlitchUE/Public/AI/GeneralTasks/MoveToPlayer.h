// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/SightComponent.h"
#include "MoveToPlayer.generated.h"

UCLASS()
class GLITCHUE_API UMoveToPlayer : public UBTTaskNode{
	GENERATED_BODY()

public:
	UMoveToPlayer();

private:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	bool bUseSight = true;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	float LooseTime = 3;

	UPROPERTY(EditAnywhere, Category = "Player")
	FBlackboardKeySelector Player;

	UPROPERTY(EditAnywhere, Category = "Player")
	FBlackboardKeySelector PlayerLocation;

	AActor* TargetActor;

	AAIController* CurrentController;

	APawn* CurrentPawn;

	USightComponent* SightComponent;

	UBlackboardComponent* CurrentBlackboard;

	UBehaviorTreeComponent* BTOwnerComp;

	FTimerHandle MoveToTimer;

	FTimerHandle TimerPlayerInSight;

	FTimerHandle TimerLoosePlayer;

	void MoveToPlayer();

	void MoveTask();

	void CheckIfPlayerIsInSight();

	void FindPlayer();

	void EndTask(const EBTNodeResult::Type TaskResult);
};
