// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "LookAt.generated.h"

UCLASS()
class GLITCHUE_API ULookAt : public UBTTaskNode{
	GENERATED_BODY()

public:
	ULookAt();
	
private:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	FBlackboardKeySelector LookAtPosition;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	bool bIgnorePitchRotation = true;
};
