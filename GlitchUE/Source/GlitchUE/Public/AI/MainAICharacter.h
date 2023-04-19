// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainAIController.h"
#include "MainAIData.h"
#include "Components/HealthComponent.h"
#include "Components/SightComponent.h"
#include "Components/WidgetComponent.h"
#include "PlacableObject/Trap.h"
#include "MainAICharacter.generated.h"

class AWaveManager;

UCLASS()
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

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sight")
	UWidgetComponent* SightWidget;

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
	void ReceiveTrapEffect(const ETrapEffect NewEffect, const float EffectDuration, const float EffectTickRate, const float EffectDamages);

private:
	FTimerHandle EffectTimer;

	FTimerHandle TrapTimer;
};
