// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackTarget.h"
#include "AttackPlayer.generated.h"

UCLASS()
class GLITCHUE_API UAttackPlayer : public UAttackTarget{
	GENERATED_BODY()

protected:
	virtual void Attack(APawn* AIPawn, AMainAIController* AIController, AActor* Target) override;
};
