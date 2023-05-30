// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainAIController.h"
#include "Components/HealthComponent.h"
#include "Components/SightComponent.h"
#include "Components/WidgetComponent.h"
#include "PlacableObject/Trap.h"
#include "PlacableObject/TrapData.h"
#include "MainAICharacter.generated.h"

class AWaveManager;

UCLASS(Abstract)
class GLITCHUE_API AMainAICharacter : public ACharacter, public IGlitchInterface{
	GENERATED_BODY()

public:
	AMainAICharacter();

	virtual void SetCurrentData(UMainAIData* NewData);

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	AMainAIController* AIController;

	UMainAIData* CurrentData;

	UPROPERTY(BlueprintReadWrite)
	UBlackboardComponent* Blackboard;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UPopcornFXEmitterComponent* IdleFX;

public:
	UBlackboardComponent* GetBlackBoard() const;

	UPopcornFXEmitterComponent* GetIdleFX() const;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UHealthComponent* HealthComp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	USightComponent* SightComp;

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

	virtual void ReceiveGlitchUpgrade() override;

	virtual void ResetGlitchUpgrade() override;

	UFUNCTION(BlueprintCallable)
	void ReceiveTrapEffect(const UTrapData* TrapData);

protected:
	void ResetTrapEffect();

	void ReceiveBurnEffect(const float EffectDuration, const float EffectTickRate, const float EffectDamages);

	void ResetBurnEffect();

	void ReceiveFreezeEffect(const float EffectDuration);

	void ResetFreezeEffect();

	void ReceivePoisonEffect(const float EffectDuration);

	void ResetPoisonEffect();

	void ReceiveSlowEffect(const float EffectDuration, const float SlowSpeed);

	void ResetSlowEffect();

	FTimerHandle EffectTimer;

	FTimerHandle TrapTimer;

	UPROPERTY()
	UPopcornFXEmitterComponent* TrapEffectFX;
};
