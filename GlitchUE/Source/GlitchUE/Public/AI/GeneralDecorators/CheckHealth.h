// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomDecorator.h"
#include "DecoratorEnums.h"
#include "CheckHealth.generated.h"

UCLASS()
class GLITCHUE_API UCheckHealth : public UCustomDecorator{
	GENERATED_BODY()

public:
	UCheckHealth();

private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector DestructibleObject;

	UPROPERTY(EditAnywhere, Category = "Check")
	ECheckMethod CheckMethod;

	UPROPERTY(EditAnywhere, Category = "Check")
	float CheckValue = 0;

	UPROPERTY(EditAnywhere, Category = "Check")
	bool bInvertResult = false;
};
