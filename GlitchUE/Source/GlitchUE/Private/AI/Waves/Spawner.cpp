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

void ASpawner::BeginPlay(){
	TArray<AActor*> WaveManagerTemp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaveManager::StaticClass(), WaveManagerTemp);
	WaveManager = Cast<AWaveManager>(WaveManagerTemp[0]);
	Gamemode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(this));
}

void ASpawner::BeginSpawn(int NumberToSpawn, TSubclassOf<AMainAICharacter> AIToSpawn) {
	CurrentAITOSpawn = AIToSpawn;
	NumberOfAISpawn = NumberToSpawn;

	CurrentNumberOfAISpawned = 0;

	SpawnAI();
}


void ASpawner::SpawnAI() {
	AMainAICharacter* NewPawn = GetWorld()->SpawnActor<AMainAICharacter>(CurrentAITOSpawn, GetActorLocation(), GetActorRotation(), ActorSpawnParameters);
	NewPawn->SpawnDefaultController();
	NewPawn->SetWaveManager(WaveManager);
	WaveManager->AddAIToList(NewPawn);
	AMainAIController* AIController = Cast<AMainAIController>(NewPawn->Controller);
	Gamemode->AddGlitch(NewPawn->GetMainAIController()->AISpawnGlitchValue);

	CurrentNumberOfAISpawned++;

	if (AnyAILeftToSpawn()) {
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
			SpawnAI();
		}, SpawnDelay, false);
	}
}

UActivableComponent* ASpawner::GetActivableComp(){
	return ActivableComp;
}

bool ASpawner::AnyAILeftToSpawn(){
	return CurrentNumberOfAISpawned < NumberOfAISpawn;
}
