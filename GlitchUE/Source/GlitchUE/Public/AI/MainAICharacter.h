// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainAIController.h"
#include "Components/HealthComponent.h"
#include "PlacableObject/Trap.h"
#include "MainAICharacter.generated.h"

class AWaveManager;

UCLASS()
class GLITCHUE_API AMainAICharacter : public ACharacter{
	GENERATED_BODY()

public:
	AMainAICharacter();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void InitializeAI(FTransform NewTransform, UBlackboardData* NewBlackBoard);

	AMainAIController* AIController;

	UBlackboardComponent* Blackboard;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UHealthComponent* HealthComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float GlitchUpgradeDuration;
	
	UFUNCTION()
	void HealthNull();

	AWaveManager* WaveManager;

	ETrapEffect CurrentTrapEffect = ETrapEffect::None;

public:
	UFUNCTION(BlueprintCallable)
	void StunAI();

	void SetWaveManager(AWaveManager* NewWaveManager);

	AMainAIController* GetMainAIController() const;

	UHealthComponent* GetHealthComp() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GlitchUpgrade();
	virtual void GlitchUpgrade_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetGlitchUpgrade();
	virtual void ResetGlitchUpgrade_Implementation();
	
	UFUNCTION(BlueprintCallable)
	void ReceiveTrapEffect(const ETrapEffect NewEffect, const float EffectDuration, const float EffectTickRate, const float EffectDamages);

private:
	FTimerHandle EffectTimer;

	FTimerHandle TrapTimer;
};
