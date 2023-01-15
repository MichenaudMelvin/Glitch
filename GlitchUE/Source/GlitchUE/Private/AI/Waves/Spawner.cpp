// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Waves/Spawner.h"
#include "Engine/World.h"
#include "AI/MainAIController.h"
#include "AI/Waves/WaveManager.h"
#include "GlitchUEGameMode.h"
#include "Kismet/GameplayStatics.h"

ASpawner::ASpawner(){
	PrimaryActorTick.bCanEverTick = false;
	SpawnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerMesh"));

	SpawnerMesh->SetMobility(EComponentMobility::Static);

	ActivableComp = CreateDefaultSubobject<UActivableComponent>(TEXT("ActivableComp"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Engine/EditorMeshes/EditorCylinder"));
	check(Mesh.Succeeded());

	SpawnerMesh->SetStaticMesh(Mesh.Object);
}

void ASpawner::Spawn(int numberToSpawn, TSubclassOf<AMainAICharacter> AIToSpawn){
	FActorSpawnParameters ActorSpawnParameters;
	AGlitchUEGameMode* Gamemode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(this));

	for (int i = 0; i < numberToSpawn; i++) {
		AMainAICharacter* NewPawn = GetWorld()->SpawnActor<AMainAICharacter>(AIToSpawn, GetActorLocation(), GetActorRotation(), ActorSpawnParameters);
		NewPawn->SpawnDefaultController();
		NewPawn->SetWaveManager(WaveManager);
		WaveManager->AddAIToList(NewPawn);
		AMainAIController* AIController = Cast<AMainAIController>(NewPawn->Controller);
		Gamemode->AddGlitch(NewPawn->GetMainAIController()->AISpawnGlitchValue);
	}
}

UActivableComponent* ASpawner::GetActivableComp(){
	return ActivableComp;
}