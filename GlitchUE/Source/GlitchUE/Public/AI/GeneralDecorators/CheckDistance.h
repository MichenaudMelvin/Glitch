// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomDecorator.h"
#include "DecoratorEnums.h"
#include "CheckDistance.generated.h"

UCLASS()
class GLITCHUE_API UCheckDistance : public UCustomDecorator{
	GENERATED_BODY()

public:
	UCheckDistance();

private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "Distance")
	FBlackboardKeySelector Actor;

	UPROPERTY(EditAnywhere, Category = "Distance")
	FBlackboardKeySelector Distance;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	ECheckMethod CheckMethod;
};
