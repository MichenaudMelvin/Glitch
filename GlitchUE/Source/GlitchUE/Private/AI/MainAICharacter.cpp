// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MainAICharacter.h"

AMainAICharacter::AMainAICharacter(){
	PrimaryActorTick.bCanEverTick = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("AIHealth"));
}

void AMainAICharacter::BeginPlay(){
	Super::BeginPlay();
	
	AIController = Cast<AMainAIController>(GetController());
	Blackboard = AIController->GetBlackboardComponent();
	Blackboard->SetValueAsVector(FName(TEXT("OriginalPosition")), GetActorLocation());

	AIControllerClass;
}

void AMainAICharacter::StunAI() {
	Blackboard->SetValueAsBool(FName(TEXT("IsStun")), true);
}