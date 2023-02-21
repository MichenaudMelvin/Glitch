// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SetBoolKey.generated.h"

UCLASS()
class GLITCHUE_API USetBoolKey : public UBTService{
	GENERATED_BODY()

public:
	USetBoolKey();

private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Bool")
	bool bBoolValue;
	
	UPROPERTY(EditAnywhere, Category = "Bool")
	FBlackboardKeySelector BoolKey;
};
