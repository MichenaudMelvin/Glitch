// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Objectives/Catalyseur.h"
#include "SetNearestCatalyseur.generated.h"

UCLASS()
class GLITCHUE_API USetNearestCatalyseur : public UBTService{
	GENERATED_BODY()

public:
	USetNearestCatalyseur();

private:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	ACatalyseur* SetFirstCatalyseurToCheck(const TArray<ACatalyseur*> CatalyseurList);
	
	UPROPERTY(EditAnywhere, Category = "Nexus")
	FBlackboardKeySelector NexusLocation;

	UPROPERTY(EditAnywhere, Category = "Catalyseur")
	FBlackboardKeySelector NearestCatalyseurLocation;

	ACharacter* OwnerPawn;
};

