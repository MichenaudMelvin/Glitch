// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "MainAIController.generated.h"

UCLASS()
class GLITCHUE_API AMainAIController : public AAIController{
	GENERATED_BODY()

public:
	AMainAIController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float StunTime = 5.0f;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float InvestigatingTime = 0.2f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAIPerceptionComponent* AIPerception;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UBlackboardData* BlackboardData;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AIPerception")
	void PerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);
	void PerceptionUpdate_Implementation(AActor* Actor, FAIStimulus Stimulus);

private:
	void SetPlayerValues(AActor* Player);
};
