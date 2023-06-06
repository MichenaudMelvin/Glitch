// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/MainAIController.h"
#include "BrainComponent.h"
#include "AI/MainAICharacter.h"
#include "AI/MainAIData.h"
#include "AI/MainAIPawn.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Helpers/FunctionsLibrary/UsefulFunctions.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionSystem.h"

AMainAIController::AMainAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))){

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

FString AMainAIController::GetControllerName() const{
	return ControllerName;
}

void AMainAIController::SetCurrentData(UMainAIData* NewData){
	AIData = NewData;
	SetDataToOwner();
}

UMainAIData* AMainAIController::GetAIData() const{
	return AIData;
}

void AMainAIController::BeginPlay(){
	Super::BeginPlay();

	if(IsValid(GetPawn())){
		ControllerName = GetPawn()->GetName() + "Controller";
		SetDataToOwner();
	}

	const FString Settings = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->OptionsString;

#if WITH_EDITOR
	// for simulation mode
	if(Settings == "?SpectatorOnly=1"){
		InitializeAIFromStart();
		return;
	}
#endif

#if !UE_BUILD_SHIPPING
	// for launch game
	if(Settings == "?Name=Player"){
		if(!IsValid(GetPawn()) || !IsValid(AIData)){
			FTimerHandle TimerHandle;

			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMainAIController::InitializeAIFromStart,0.01f, false);
			return;
		}

		InitializeAIFromStart();
		return;
	}
#endif

	if(Settings != ""){
		TArray<FString> LevelSettings;
		Settings.ParseIntoArray(LevelSettings, TEXT("|"), true);

		const UAbstractSave* LoadedSave = UUsefulFunctions::LoadSave(UWorldSave::StaticClass(), FCString::Atoi(*LevelSettings[1]), false);

		if(!IsValid(LoadedSave)){
			return;
		}

		if(LevelSettings[0] == "?WorldSaveLoad" && LoadedSave->IsA(UStealthSave::StaticClass())){
			//InitializeAI will be call by the gamemode
			return;
		}
	}

	if(!IsValid(GetPawn()) || !IsValid(AIData)){
		FTimerHandle TimerHandle;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMainAIController::InitializeAIFromStart,0.01f, false);
		return;
	}

	InitializeAIFromStart();
}

void AMainAIController::SetDataToOwner(){
	Damages = AIData->Damages;

	if(GetPawn()->IsA(AMainAICharacter::StaticClass())){
		Cast<AMainAICharacter>(GetPawn())->SetCurrentData(AIData);
	} else if(GetPawn()->IsA(AMainAIPawn::StaticClass())){
		//Cast<AMainAIPawn>(GetPawn())->SetCurrentData(AIData);
	}
}

void AMainAIController::InitializeAIFromStart(){
	RunBehaviorTree(BehaviorTree);
	UseBlackboard(BlackboardData, Blackboard);

	Blackboard->SetValueAsFloat("StunTime", AIData->StunTime);
	Blackboard->SetValueAsFloat("InvestigatingTime", AIData->InvestigatingTime);

	Blackboard->SetValueAsVector("OriginalPosition", GetPawn()->GetActorLocation());
	Blackboard->SetValueAsRotator("OriginalRotation", GetPawn()->GetActorRotation());
}

void AMainAIController::ToggleGlitchDamages(const bool bEnable){
	Damages = bEnable ? AIData->GlitchDamages : AIData->Damages;
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
	CurrentData.OriginalRotation = Blackboard->GetValueAsRotator("OriginalRotation");
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

	Blackboard->SetValueAsFloat("StunTime", AIData->StunTime);
	Blackboard->SetValueAsFloat("InvestigatingTime", AIData->InvestigatingTime);

	Blackboard->SetValueAsVector("OriginalPosition", NewData.OriginalPosition);
	Blackboard->SetValueAsRotator("OriginalRotation", NewData.OriginalRotation);
	Blackboard->SetValueAsBool("IsStun", NewData.bIsStun);
	Blackboard->SetValueAsBool("Investigate", NewData.bInvestigate);

	Blackboard->SetValueAsVector("InvestigationLocation", NewData.InvestigationLocation);
	Blackboard->SetValueAsBool("DoingExternalActions", NewData.bDoingExternalActions);
	Blackboard->SetValueAsBool("ReceiveAlert", NewData.bReceiveAlert);
}
