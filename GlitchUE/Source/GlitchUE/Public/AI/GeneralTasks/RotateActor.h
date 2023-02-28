// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RotateActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FKOnRotate, APawn*, Pawn, UBehaviorTreeComponent*, OwnerComp);

UCLASS()
class GLITCHUE_API URotateActor : public UBTTaskNode{
	GENERATED_BODY()

public:
	URotateActor();

private:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category = "Behavior")
	FBlackboardKeySelector CameraBehavior;

	UPROPERTY(EditAnywhere, Category = "Speed")
	FBlackboardKeySelector SpeedFactor;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	FBlackboardKeySelector MaxRotation;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	FBlackboardKeySelector MinRotation;

	FRotator StartRotation;

	FRotator TargetRotation = FRotator::ZeroRotator;

	bool bPositivRotation = true;

	FKOnRotate OnRotate;

	UFUNCTION()
	void ClampRotation(APawn* PawnToRotate, UBehaviorTreeComponent* OwnerComp);

	UFUNCTION()
	void FullRotation(APawn* PawnToRotate, UBehaviorTreeComponent* OwnerComp);
};
