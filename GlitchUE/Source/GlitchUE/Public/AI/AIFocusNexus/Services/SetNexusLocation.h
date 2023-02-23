// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SetNexusLocation.generated.h"

UCLASS()
class GLITCHUE_API USetNexusLocation : public UBTService{
	GENERATED_BODY()

public:
	USetNexusLocation();

private:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Nexus")
	FBlackboardKeySelector NexusLocation;
};
