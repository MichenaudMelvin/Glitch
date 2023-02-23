// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "UpdateValue.generated.h"

UENUM(BlueprintType)
enum class EUpdateMethod : uint8{
	Increment,

	Decrement,
};

UCLASS()
class GLITCHUE_API UUpdateValue : public UBTService{
	GENERATED_BODY()

public:
	UUpdateValue();

private:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Value")
	FBlackboardKeySelector ValueToUpdate;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	EUpdateMethod UpdateMethod;
};