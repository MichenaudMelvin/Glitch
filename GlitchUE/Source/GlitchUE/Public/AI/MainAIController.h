// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Objectives/Catalyseur.h"
#include "MainAIController.generated.h"

UCLASS()
class GLITCHUE_API AMainAIController : public AAIController{
	GENERATED_BODY()

public:
	AMainAIController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Glitch")
	float AISpawnGlitchValue = 25;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float StunTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damages")
	float Damages = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Glitch")
	float GlitchDamages = 25;

	float OriginalDamages;

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

public:
	void ToggleGlitchDamages(const bool bEnable);

	UFUNCTION(BlueprintCallable)
	float GetDamages() const;

	virtual void SwitchBehavior(UBehaviorTree* NewBehaviorTree, UBlackboardData* NewBlackboardData);

	// empty function only for two classes
	virtual TArray<ACatalyseur*> GetCatalyseurList() const;
};
