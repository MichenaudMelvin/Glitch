// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomDecorator.h"
#include "GlitchUEGameMode.h"
#include "CheckPhase.generated.h"

UCLASS()
class GLITCHUE_API UCheckPhase : public UCustomDecorator{
	GENERATED_BODY()

public:
	UCheckPhase();

private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "Phases")
	EPhases TargetPhase;
};
