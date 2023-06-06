// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AI/MainAIController.h"
#include "AttackTarget.generated.h"

UCLASS()
class GLITCHUE_API UAttackTarget : public UBTTaskNode{
	GENERATED_BODY()

public:
	UAttackTarget();

private:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UFUNCTION()
	void OnFinishAttackFX(UPopcornFXEmitterComponent* EmitterComponent, FVector Location, FVector Rotation);

	virtual void Attack(APawn* AIPawn, AMainAIController* AIController, AActor* Target);

	void PlayAttackFX(APawn* AIPawn, const AActor* Target);

	UPROPERTY(EditAnywhere, Category = "Attack")
	FBlackboardKeySelector TargetToAttack;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FBlackboardKeySelector AttackKey;

	UBlackboardComponent* CurrentBlackboard;

	UBehaviorTreeComponent* CurrentOwnerComp;
};
