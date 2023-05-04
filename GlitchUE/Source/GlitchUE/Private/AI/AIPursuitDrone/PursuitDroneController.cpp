// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AIPursuitDrone/PursuitDroneController.h"
#include "AI/AIPursuitDrone/PursuitDrone.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Objectives/Nexus.h"

APursuitDroneController::APursuitDroneController(const FObjectInitializer& ObjectInitializer) : AMainAIController(ObjectInitializer){
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeAsset(TEXT("/Game/Blueprint/AI/AIPursuitDrone/BT_PursuitDrone"));
	check(BehaviorTreeAsset.Succeeded());

	BehaviorTree = BehaviorTreeAsset.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardAsset(TEXT("/Game/Blueprint/AI/AIPursuitDrone/BB_PursuitDrone"));
	check(BlackboardAsset.Succeeded());

	BlackboardData = BlackboardAsset.Object;
}

void APursuitDroneController::BeginPlay(){
	Super::BeginPlay();
}

void APursuitDroneController::InitializeAIFromStart(){
	Super::InitializeAIFromStart();

	Blackboard->SetValueAsBool("IsDocked", true);
	Cast<APursuitDrone>(GetPawn())->ForceInDock();

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if(IsValid(Player)){
		Blackboard->SetValueAsObject("Player", Player);
		Blackboard->SetValueAsVector("PlayerLocation", Player->GetActorLocation());
	}
}

void APursuitDroneController::InitializeAI(const FAIData NewData){
	Super::InitializeAI(NewData);

	Blackboard->SetValueAsBool("IsDocked", NewData.bIsDocked);

	if(NewData.bIsDocked){
		Cast<APursuitDrone>(GetPawn())->ForceInDock();
	} else{
		Cast<APursuitDrone>(GetPawn())->ForceStartAnim();
	}

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if(IsValid(Player)){
		Blackboard->SetValueAsObject("Player", Player);
		Blackboard->SetValueAsVector("PlayerLocation", Player->GetActorLocation());
	}
}

void APursuitDroneController::SwitchBehavior(UBehaviorTree* NewBehaviorTree, UBlackboardData* NewBlackboardData){
	Super::SwitchBehavior(NewBehaviorTree, NewBlackboardData);

	APursuitDrone* CurrentDrone = Cast<APursuitDrone>(GetPawn());

	GetCharacter()->GetCapsuleComponent()->OnComponentBeginOverlap.Clear();
	CurrentDrone->GetInteractableComp()->OnInteract.AddDynamic(CurrentDrone, &APursuitDrone::Interact);
	CurrentDrone->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	CurrentDrone->GetInteractableComp()->AddInteractable(CurrentDrone->GetCapsuleComponent());

	TArray<AActor*> NexusArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANexus::StaticClass(), NexusArray);
	ANexus* Nexus = Cast<ANexus>(NexusArray[0]);

	TArray<AActor*> CatalyseurArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACatalyseur::StaticClass(), CatalyseurArray);
	for(int i = 0; i < CatalyseurArray.Num(); i++){
		CatalyseurList.Add(Cast<ACatalyseur>(CatalyseurArray[i]));
	}

	Blackboard->SetValueAsObject("Nexus", Nexus);
	Blackboard->SetValueAsVector("NexusLocation", Nexus->GetActorLocation());
}

FAIData APursuitDroneController::SaveAI(){
	FAIData CurrentData = Super::SaveAI();

	CurrentData.bIsDocked = Blackboard->GetValueAsBool("IsDocked");

	return CurrentData;
}

TArray<ACatalyseur*> APursuitDroneController::GetCatalyseurList() const{
	return CatalyseurList;
}
