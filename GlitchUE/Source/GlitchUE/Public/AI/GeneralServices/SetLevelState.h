// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GlitchUEGameMode.h"
#include "SetLevelState.generated.h"

UCLASS()
class GLITCHUE_API USetLevelState : public UBTService{
	GENERATED_BODY()

public:
	USetLevelState();

private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "LevelState")
	ELevelState NewLevelState;

};
