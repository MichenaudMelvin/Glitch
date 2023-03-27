// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/MainAIController.h"
#include "BrainComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
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

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMainAIController::PerceptionUpdate);

	OriginalDamages = Damages;

	const FString Settings = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->OptionsString;

	if(Settings != ""){
		TArray<FString> LevelSettings;
		Settings.ParseIntoArray(LevelSettings, TEXT("|"), true);

		if(LevelSettings[0] == "?WorldSaveLoad"){
			//InitializeAI will be call by the gamemode
			return;
		}
	}

	InitializeAIFromStart();
}

void AMainAIController::InitializeAIFromStart(){
	RunBehaviorTree(BehaviorTree);
	UseBlackboard(BlackboardData, Blackboard);

	Blackboard->SetValueAsFloat("StunTime", StunTime);
	Blackboard->SetValueAsFloat("InvestigatingTime", InvestigatingTime);

	FTimerHandle TimerHandle;

	// micro delay pour les AI qui spawnent
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){
		Blackboard->SetValueAsVector("OriginalPosition", GetPawn()->GetActorLocation());
	}, 0.2f, false);
}

void AMainAIController::PerceptionUpdate_Implementation(AActor* Actor, const FAIStimulus Stimulus) {
	UE_LOG(LogTemp, Error, TEXT("c'est un print pour savoir si cette fonction est encore utile"));
	// if (UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus) == UAISense_Sight::StaticClass()) {
	// 	if (Actor->IsA(AMainPlayer::StaticClass())) {
	// 		AActor* Player = Actor;
	//
	// 		if (IsValid(Blackboard->GetValueAsObject(FName(TEXT("Player"))))) {
	// 			SetPlayerValues(Player);
	// 		} 
	// 		else {
	// 			Blackboard->SetValueAsBool(FName(TEXT("Investigate")), true);
	// 			Blackboard->SetValueAsVector(FName(TEXT("InvestigationLocation")), Player->GetActorLocation());
	//
	// 			FTimerHandle TimerHandle;
	//
	// 			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
	// 				SetPlayerValues(Player);
	// 			}, Blackboard->GetValueAsFloat(FName(TEXT("InvestigatingTime"))), false);
	// 		}
	// 	}
	// }
}

void AMainAIController::ToggleGlitchDamages(const bool bEnable){
	Damages = bEnable ? GlitchDamages : OriginalDamages; 
}

float AMainAIController::GetDamages() const{
	return Damages;
}

void AMainAIController::SwitchBehavior(UBehaviorTree* NewBehaviorTree, UBlackboardData* NewBlackboardData){
	GetBrainComponent()->StopLogic("Switch Behavior");

	BehaviorTree = NewBehaviorTree;
	BlackboardData = NewBlackboardData;

	RunBehaviorTree(BehaviorTree);
	UseBlackboard(BlackboardData, Blackboard);
}

TArray<ACatalyseur*> AMainAIController::GetCatalyseurList() const{
	TArray<ACatalyseur*> EmptyList;
	return EmptyList;
}

FAIData AMainAIController::SaveAI(){
	FAIData CurrentData;

	CurrentData.CurrentTransform = GetPawn()->GetActorTransform();

	CurrentData.OriginalPosition = Blackboard->GetValueAsVector("OriginalPosition");
	CurrentData.bIsStun = Blackboard->GetValueAsBool("IsStun");
	CurrentData.bInvestigate = Blackboard->GetValueAsBool("Investigate");
	CurrentData.InvestigationLocation = Blackboard->GetValueAsVector("InvestigationLocation");
	CurrentData.bDoingExternalActions = Blackboard->GetValueAsBool("DoingExternalActions");
	CurrentData.bReceiveAlert = Blackboard->GetValueAsBool("ReceiveAlert");

	return CurrentData;
}

void AMainAIController::InitializeAI(const FAIData NewData){
	RunBehaviorTree(BehaviorTree);
	UseBlackboard(BlackboardData, Blackboard);

	GetPawn()->SetActorTransform(NewData.CurrentTransform);

	Blackboard->SetValueAsFloat("StunTime", StunTime);
	Blackboard->SetValueAsFloat("InvestigatingTime", InvestigatingTime);

	Blackboard->SetValueAsVector("OriginalPosition", NewData.OriginalPosition);
	Blackboard->SetValueAsBool("IsStun", NewData.bIsStun);
	Blackboard->SetValueAsBool("Investigate", NewData.bInvestigate);

	Blackboard->SetValueAsVector("InvestigationLocation", NewData.InvestigationLocation);
	Blackboard->SetValueAsBool("DoingExternalActions", NewData.bDoingExternalActions);
	Blackboard->SetValueAsBool("ReceiveAlert", NewData.bReceiveAlert);
}
