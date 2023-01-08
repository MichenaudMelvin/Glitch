// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/MainAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Player/MainPlayer.h"

AMainAIController::AMainAIController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) {

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void AMainAIController::BeginPlay() {
	RunBehaviorTree(BehaviorTree);

	AIPerception = NewObject<UAIPerceptionComponent>();

	Blackboard->SetValueAsFloat(FName(TEXT("StunTime")), StunTime);
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMainAIController::PerceptionUpdate);
}

void AMainAIController::PerceptionUpdate_Implementation(AActor* Actor, FAIStimulus Stimulus) {
	if (UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus) == UAISense_Sight::StaticClass()) {
		if (Cast<AMainPlayer>(Actor)) {
			if (IsValid(Blackboard->GetValueAsObject(FName(TEXT("Player"))))) {
				SetPlayerValues(Actor);
			} 
			else {
				Blackboard->SetValueAsBool(FName(TEXT("Player")), true);
				Blackboard->SetValueAsVector(FName(TEXT("InvestigationLocation")), Actor->GetActorLocation());
				
				FTimerHandle TimerHandle;

				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
					SetPlayerValues(Actor);
				}, InvestigatingTime, false);
			}
		}
	}
}

void AMainAIController::SetPlayerValues(AActor* Player) {
	Blackboard->SetValueAsObject(FName(TEXT("Player")), Player);
	Blackboard->SetValueAsVector(FName(TEXT("PlayerLocation")), Player->GetActorLocation());
}