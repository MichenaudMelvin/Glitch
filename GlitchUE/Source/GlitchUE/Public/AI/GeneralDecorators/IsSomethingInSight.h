// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomDecorator.h"
#include "IsSomethingInSight.generated.h"

UCLASS()
class GLITCHUE_API UIsSomethingInSight : public UCustomDecorator{
	GENERATED_BODY()

private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	bool bInvertResult = false;
};
