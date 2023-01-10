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

	//AIPerception = NewObject<UAIPerceptionComponent>();

	//UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), (IsValid(AIPerception) ? TEXT("true") : TEXT("false")));

	Blackboard->SetValueAsFloat(FName(TEXT("StunTime")), StunTime);
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMainAIController::PerceptionUpdate);
	
	//RunBehaviorTree(BehaviorTree);
}

void AMainAIController::PerceptionUpdate_Implementation(AActor* Actor, FAIStimulus Stimulus) {
	/*
	UE_LOG(LogTemp, Warning, TEXT("Hello"));
	if (UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus) == UAISense_Sight::StaticClass()) {
		UE_LOG(LogTemp, Warning, TEXT("STIMULUS == SIGHT"));
		if (Cast<AMainPlayer>(Actor)) {
			UE_LOG(LogTemp, Warning, TEXT("CAST"));
			if (IsValid(Blackboard->GetValueAsObject(FName(TEXT("Player"))))) {
				UE_LOG(LogTemp, Warning, TEXT("GET BB VAL"));
				SetPlayerValues(Actor);
			} 
			else {
				UE_LOG(LogTemp, Warning, TEXT("ELSE"));
				Blackboard->SetValueAsBool(FName(TEXT("Player")), true);
				Blackboard->SetValueAsVector(FName(TEXT("InvestigationLocation")), Actor->GetActorLocation());
				
				FTimerHandle TimerHandle;

				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
					SetPlayerValues(Actor);
				}, InvestigatingTime, false);
			}
		}
	}*/
}

void AMainAIController::SetPlayerValues(AActor* Player) {
	Blackboard->SetValueAsObject(FName(TEXT("Player")), Player);
	Blackboard->SetValueAsVector(FName(TEXT("PlayerLocation")), Player->GetActorLocation());
}