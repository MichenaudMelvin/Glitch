// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomDecorator.h"
#include "IsPlayerInGlitchZone.generated.h"

UCLASS()
class GLITCHUE_API UIsPlayerInGlitchZone : public UCustomDecorator{
	GENERATED_BODY()

public:
	UIsPlayerInGlitchZone();

private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "Player")
	FBlackboardKeySelector Player;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	bool bInvertResult = false;
};
