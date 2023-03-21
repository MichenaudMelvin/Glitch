// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MainAIPawn.h"
#include "AI/MainAIController.h"

AMainAIPawn::AMainAIPawn(){
	PrimaryActorTick.bCanEverTick = false;

	AIMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AIMesh"));
	SetRootComponent(AIMesh);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMainAIPawn::BeginPlay(){
	Super::BeginPlay();

	Blackboard = Cast<AMainAIController>(GetController())->GetBlackboardComponent();

	Blackboard->SetValueAsVector("OriginalPosition", GetActorLocation());
}

void AMainAIPawn::ReceiveGlitchUpgrade(){
	IGlitchInterface::ReceiveGlitchUpgrade();
}

void AMainAIPawn::ResetGlitchUpgrade(){
	IGlitchInterface::ResetGlitchUpgrade();
}


