// Fill out your copyright notice in the Description page of Project Settings.

#include "Navigation/CrowdFollowingComponent.h"
#include "AI/MainAIController.h"

AMainAIController::AMainAIController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) {

}

void AMainAIController::BeginPlay() {
	RunBehaviorTree(BehaviorTree);

}