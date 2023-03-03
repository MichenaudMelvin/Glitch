// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPursuitDrone/PursuitDroneController.h"
#include "BehaviorTree/BlackboardComponent.h" 	
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Kismet/GameplayStatics.h"

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
	
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if(IsValid(Player)){
		Blackboard->SetValueAsObject(FName(TEXT("Player")), Player);
		Blackboard->SetValueAsVector(FName(TEXT("PlayerLocation")), Player->GetActorLocation());
	}
}
