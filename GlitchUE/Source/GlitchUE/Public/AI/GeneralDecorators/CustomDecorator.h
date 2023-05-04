// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CustomDecorator.generated.h"

UCLASS()
class GLITCHUE_API UCustomDecorator : public UBTDecorator{
	GENERATED_BODY()

public:
	UCustomDecorator();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void CheckAbort(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
