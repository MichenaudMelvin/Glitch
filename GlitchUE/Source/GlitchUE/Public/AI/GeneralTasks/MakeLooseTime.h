// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Player/MainPlayerController.h"
#include "MakeLooseTime.generated.h"

UCLASS()
class GLITCHUE_API UMakeLooseTime : public UBTTaskNode{
	GENERATED_BODY()

private:
	virtual void PostLoad() override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY()
	AGlitchUEGameMode* GameMode;

	UPROPERTY(EditAnywhere, Category = "Timer")
	float LooseTime = 10.0f;
};
