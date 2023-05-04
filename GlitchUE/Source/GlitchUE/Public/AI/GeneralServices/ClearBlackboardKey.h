// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ClearBlackboardKey.generated.h"

UCLASS()
class GLITCHUE_API UClearBlackboardKey : public UBTService{
	GENERATED_BODY()

public:
	UClearBlackboardKey();

protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Key")
	FBlackboardKeySelector Key;
};
