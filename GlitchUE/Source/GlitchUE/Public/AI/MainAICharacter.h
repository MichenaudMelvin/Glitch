// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HealthComponent.h"
#include "MainAICharacter.generated.h"

UCLASS()
class GLITCHUE_API AMainAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainAICharacter();

protected:
	virtual void BeginPlay() override;

	AMainAIController* AIController;

	UBlackboardComponent* Blackboard;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UHealthComponent* HealthComp;

public:
	UFUNCTION(BlueprintCallable)
	void StunAI();

};
