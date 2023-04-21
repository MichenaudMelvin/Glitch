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

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &APatrolController::PerceptionUpdate);

	UAISenseConfig_Hearing* ConfigHearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));

	FAISenseAffiliationFilter AffiliationFilter;

	AffiliationFilter.bDetectEnemies = true;
	AffiliationFilter.bDetectFriendlies = true;
	AffiliationFilter.bDetectNeutrals = true;

	ConfigHearing->DetectionByAffiliation = AffiliationFilter;
	AIPerception->ConfigureSense(*ConfigHearing);
}

void APatrolController::InitializeAIFromStart(){
	Super::InitializeAIFromStart();

	Blackboard->SetValueAsInt("CurrentIndex", 0);
	Blackboard->SetValueAsObject("CurrentPatrolActor", Cast<APatrolCharacter>(GetPawn())->GetPatrolPointList()[0]);
}

FAIData APatrolController::SaveAI(){
	FAIData CurrentData = Super::SaveAI();

	CurrentData.CurrentIndex = Blackboard->GetValueAsInt("CurrentIndex");

	return CurrentData;
}

void APatrolController::InitializeAI(const FAIData NewData){
	Super::InitializeAI(NewData);

	Blackboard->SetValueAsInt("CurrentIndex", NewData.CurrentIndex);
	Blackboard->SetValueAsObject("CurrentPatrolActor", Cast<APatrolCharacter>(GetPawn())->GetPatrolPointList()[NewData.CurrentIndex]);
}

void APatrolController::PerceptionUpdate(AActor* Actor, const FAIStimulus Stimulus){
	if(Actor == this){
		return;
	}

	if (UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus) == UAISense_Hearing::StaticClass()){
		if(Blackboard->GetValueAsBool("HearSound")){
			Blackboard->SetValueAsBool("IsMovingToHearingLocation", false);
		} else{
			Blackboard->SetValueAsBool("HearSound", true);
		}

		Blackboard->SetValueAsVector("HearingLocation", Stimulus.StimulusLocation);
	}
}
