// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPatrol/PatrolController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

APatrolController::APatrolController(const FObjectInitializer& ObjectInitializer) : AMainAIController(ObjectInitializer){
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeAsset(TEXT("/Game/Blueprint/AI/AIPatrol/BT_Patrol"));
	check(BehaviorTreeAsset.Succeeded());

	BehaviorTree = BehaviorTreeAsset.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardAsset(TEXT("/Game/Blueprint/AI/AIPatrol/BB_Patrol"));
	check(BlackboardAsset.Succeeded());
	
	BlackboardData = BlackboardAsset.Object;
}
