// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Objectives/Catalyseur.h"
#include "Saves/WorldSave.h"
#include "MainAIController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnStopBehavior);

class UMainAIData;

UCLASS()
class GLITCHUE_API AMainAIController : public AAIController{
	GENERATED_BODY()

public:
	AMainAIController(const FObjectInitializer& ObjectInitializer);

	FKOnStopBehavior OnStopBehavior;

	FString GetControllerName() const;

	void SetCurrentData(UMainAIData* NewData);

	UMainAIData* GetAIData() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UMainAIData* AIData;

	void SetDataToOwner();

	// needed because controller is instanced
	UPROPERTY(BlueprintReadOnly)
	FString ControllerName;

	virtual void InitializeAIFromStart();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	float Damages;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAIPerceptionComponent* AIPerception;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UBlackboardData* BlackboardData;

	UFUNCTION()
	void PerceptionUpdate(AActor* Actor, const FAIStimulus Stimulus);

public:
	void ToggleGlitchDamages(const bool bEnable);

	UFUNCTION(BlueprintCallable)
	float GetDamages() const;

	virtual void SwitchBehavior(UBehaviorTree* NewBehaviorTree, UBlackboardData* NewBlackboardData);

	// empty function only for two classes
	virtual TArray<ACatalyseur*> GetCatalyseurList() const;

	virtual FAIData SaveAI();

	UFUNCTION(BlueprintCallable)
	virtual void InitializeAI(const FAIData NewData);
};
