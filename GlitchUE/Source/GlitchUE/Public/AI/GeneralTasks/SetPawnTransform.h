// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/TimelineComponent.h"
#include "SetPawnTransform.generated.h"

UCLASS()
class GLITCHUE_API USetPawnTransform : public UBTTaskNode{
	GENERATED_BODY()

public:
	USetPawnTransform();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY()
	UBehaviorTreeComponent* OwnerBehaviorTree;

	FTimeline LerpTimeline;

	UCurveFloat* ZeroToOneCurve;

	UPROPERTY(EditAnywhere, Category = "Duration")
	float TaskDuration = 1;

	FVector CurrentLocation;

	FRotator CurrentRotator;

	UPROPERTY(EditAnywhere, Category = "Location")
	FBlackboardKeySelector TargetLocation;

	FVector TargetVector;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	FBlackboardKeySelector TargetRotation;

	FRotator TargetRotator;

	APawn* TargetPawn;

	UFUNCTION()
	void UpdateLerp(float Value);

	UFUNCTION()
	void FinishLerp();
};
