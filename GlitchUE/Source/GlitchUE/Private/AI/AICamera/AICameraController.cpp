// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICamera/AICameraController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

AAICameraController::AAICameraController(const FObjectInitializer& ObjectInitializer) : AMainAIController(ObjectInitializer){
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeAsset(TEXT("/Game/Blueprint/AI/AICamera/BT_Camera"));
	check(BehaviorTreeAsset.Succeeded());
	
	BehaviorTree = BehaviorTreeAsset.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardAsset(TEXT("/Game/Blueprint/AI/AICamera/BB_Camera"));
	check(BlackboardAsset.Succeeded());
	
	BlackboardData = BlackboardAsset.Object;
}
