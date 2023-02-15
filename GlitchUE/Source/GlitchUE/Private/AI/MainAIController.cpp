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
	
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void AMainAIController::BeginPlay() {
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
	UseBlackboard(BlackboardData, Blackboard);

	//AIPerception = NewObject<UAIPerceptionComponent>();

	Blackboard->SetValueAsFloat(FName(TEXT("StunTime")), StunTime);
	Blackboard->SetValueAsFloat(FName(TEXT("InvestigatingTime")), InvestigatingTime);

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMainAIController::PerceptionUpdate);
}

void AMainAIController::PerceptionUpdate_Implementation(AActor* Actor, const FAIStimulus Stimulus) {
	if (UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus) == UAISense_Sight::StaticClass()) {
		if (Actor->IsA(AMainPlayer::StaticClass())) {
			AActor* Player = Actor;

			if (IsValid(Blackboard->GetValueAsObject(FName(TEXT("Player"))))) {
				SetPlayerValues(Player);
			} 
			else {
				Blackboard->SetValueAsBool(FName(TEXT("Investigate")), true);
				Blackboard->SetValueAsVector(FName(TEXT("InvestigationLocation")), Player->GetActorLocation());

				FTimerHandle TimerHandle;
				
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
					SetPlayerValues(Player);
				}, Blackboard->GetValueAsFloat(FName(TEXT("InvestigatingTime"))), false);
			}
		}
	}
}

void AMainAIController::SetPlayerValues(AActor* Player) {
	//Blackboard->SetValueAsObject(FName(TEXT("Player")), Player);
	//Blackboard->SetValueAsVector(FName(TEXT("PlayerLocation")), Player->GetActorLocation());
}