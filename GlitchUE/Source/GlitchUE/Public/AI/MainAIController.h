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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AIPerception")
	float AISpawnGlitchValue;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float StunTime = 5.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Damages = 100;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float InvestigatingTime = 0.2f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAIPerceptionComponent* AIPerception;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UBlackboardData* BlackboardData;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AIPerception")
	void PerceptionUpdate(AActor* Actor, const FAIStimulus Stimulus);
	void PerceptionUpdate_Implementation(AActor* Actor, const FAIStimulus Stimulus);

private:
	void SetPlayerValues(AActor* Player);
};
