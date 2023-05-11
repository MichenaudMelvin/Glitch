// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPatrol/PatrolController.h"
#include "AI/AIPatrol/PatrolCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"

APatrolController::APatrolController(const FObjectInitializer& ObjectInitializer) : AMainAIController(ObjectInitializer){
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeAsset(TEXT("/Game/Blueprint/AI/AIPatrol/BT_Patrol"));
	check(BehaviorTreeAsset.Succeeded());

	BehaviorTree = BehaviorTreeAsset.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardAsset(TEXT("/Game/Blueprint/AI/AIPatrol/BB_Patrol"));
	check(BlackboardAsset.Succeeded());

	BlackboardData = BlackboardAsset.Object;
}

void APatrolController::InitializeAIFromStart(){
	Super::InitializeAIFromStart();

	Blackboard->SetValueAsInt("CurrentIndex", 0);
	Blackboard->SetValueAsObject("CurrentPatrolActor", Cast<APatrolCharacter>(GetPawn())->GetPatrolPointList()[0]);
}

FAIData APatrolController::SaveAI(){
	FAIData CurrentData = Super::SaveAI();

	CurrentData.CurrentIndex = Blackboard->GetValueAsInt("CurrentIndex");
	CurrentData.bHearSound = Blackboard->GetValueAsBool("HearSound");
	CurrentData.HearingLocation = Blackboard->GetValueAsVector("HearingLocation");
	CurrentData.bIsMovingToHearingLocation = Blackboard->GetValueAsBool("IsMovingToHearingLocation");

	return CurrentData;
}

void APatrolController::InitializeAI(const FAIData NewData){
	Super::InitializeAI(NewData);

	Blackboard->SetValueAsInt("CurrentIndex", NewData.CurrentIndex);
	Blackboard->SetValueAsObject("CurrentPatrolActor", Cast<APatrolCharacter>(GetPawn())->GetPatrolPointList()[NewData.CurrentIndex]);
	Blackboard->SetValueAsBool("HearSound", NewData.bHearSound);
	Blackboard->SetValueAsVector("HearingLocation", NewData.HearingLocation);
	Blackboard->SetValueAsBool("IsMovingToHearingLocation", NewData.bIsMovingToHearingLocation);

}