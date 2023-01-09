// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Waves/Spawner.h"
#include "Engine/World.h"
#include "AI/MainAIController.h"

ASpawner::ASpawner(){
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SpawnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerMseh"));

	SpawnerMesh->SetupAttachment(RootComponent);
}

void ASpawner::Spawn(int numberToSpawn, TSubclassOf<AMainAICharacter> AIToSpawn){
	FActorSpawnParameters ActorSpawnParameters;

	for (int i = 0; i < numberToSpawn; i++) {
		AMainAICharacter* NewPawn = GetWorld()->SpawnActor<AMainAICharacter>(AIToSpawn, GetActorLocation(), GetActorRotation(), ActorSpawnParameters);
		NewPawn->SpawnDefaultController();
		AMainAIController* AIController = Cast<AMainAIController>(NewPawn->Controller);
	}
}

UActivableComponent* ASpawner::GetActivableComp(){
	return ActivableComp;
}

