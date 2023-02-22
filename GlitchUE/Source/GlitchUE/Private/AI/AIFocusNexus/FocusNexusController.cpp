// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIFocusNexus/FocusNexusController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

AFocusNexusController::AFocusNexusController(){
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeAsset(TEXT("/Game/Blueprint/AI/FocusAI/AIFocusNexus/BT_FocusNexus"));
	check(BehaviorTreeAsset.Succeeded());

	BehaviorTree = BehaviorTreeAsset.Object;
	
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardAsset(TEXT("/Game/Blueprint/AI/FocusAI/BB_FocusAI"));
	check(BlackboardAsset.Succeeded());
	
	BlackboardData = BlackboardAsset.Object;
}
