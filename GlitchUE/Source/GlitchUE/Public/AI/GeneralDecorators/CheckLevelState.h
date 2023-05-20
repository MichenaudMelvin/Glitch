// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomDecorator.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "CheckLevelState.generated.h"

UCLASS()
class GLITCHUE_API UCheckLevelState : public UCustomDecorator{
	GENERATED_BODY()

public:
	virtual void PostLoad() override;

private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	AGlitchUEGameMode* GlitchUEGameMode;

	UPROPERTY(EditAnywhere, Category = "LevelState")
	ELevelState TargetLevelState;
};
