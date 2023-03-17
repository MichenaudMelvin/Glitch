// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainAIController.h"
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

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void InitializeAI(FTransform NewTransform, UBlackboardData* NewBlackBoard);

	AMainAIController* AIController;

	UPROPERTY(BlueprintReadWrite)
	UBlackboardComponent* Blackboard;

public:
	UBlackboardComponent* GetBlackBoard();

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, Category = "Glitch")
	float GlitchHealth = 200;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	USightComponent* SightComp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sight")
	UWidgetComponent* SightWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Sight")
	FVector ScaleDetection = FVector(1, 1, 1);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Glitch" )
	float GlitchUpgradeDuration = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Glitch")
	float GlitchSpeed = 1000;

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	float OriginalSpeed = 200;

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
