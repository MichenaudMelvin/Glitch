// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HealthComponent.h"
#include "MainAICharacter.generated.h"

class AWaveManager;

UCLASS()
class GLITCHUE_API AMainAICharacter : public ACharacter{
	GENERATED_BODY()

public:
	AMainAICharacter();

protected:
	virtual void BeginPlay() override;

	AMainAIController* AIController;

	UBlackboardComponent* Blackboard;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UHealthComponent* HealthComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float GlitchUpgradeDuration;
	
	UFUNCTION()
	void HealthNull();

	AWaveManager* WaveManager;

public:
	UFUNCTION(BlueprintCallable)
	void StunAI();

	void SetWaveManager(AWaveManager* NewWaveManager);

	AMainAIController* GetMainAIController();

	UHealthComponent* GetHealthComp();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GlitchUpgrade();
	virtual void GlitchUpgrade_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetGlitchUpgrade();
	virtual void ResetGlitchUpgrade_Implementation();
};
