// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Waves/Spawner.h"
#include "Engine/World.h"
#include "AI/MainAIController.h"
#include "AI/Waves/WaveManager.h"
#include "GlitchUEGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"

ASpawner::ASpawner(){
	PrimaryActorTick.bCanEverTick = false;
	SpawnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerMesh"));

	SpawnerMesh->SetMobility(EComponentMobility::Static);

	SpawnerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ActivableComp = CreateDefaultSubobject<UActivableComponent>(TEXT("ActivableComp"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Engine/BasicShapes/Sphere"));
	check(Mesh.Succeeded());

	SpawnerMesh->SetStaticMesh(Mesh.Object);
}

void ASpawner::BeginPlay(){
	TArray<AActor*> WaveManagerTemp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaveManager::StaticClass(), WaveManagerTemp);
	WaveManager = Cast<AWaveManager>(WaveManagerTemp[0]);
	Gamemode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(this));

	ActivableComp->OnActivated.AddDynamic(this, &ASpawner::ActivateSpawner);
	ActivableComp->OnDesactivated.AddDynamic(this, &ASpawner::DesactivateSpawner);
}

void ASpawner::ActivateSpawner(){
	UUsefullFunctions::OutlineComponent(true, SpawnerMesh);
}

void ASpawner::DesactivateSpawner(){
	UUsefullFunctions::OutlineComponent(false, SpawnerMesh);
}

void ASpawner::BeginSpawn(int NumberToSpawn, TSubclassOf<AMainAICharacter> AIToSpawn) {
	CurrentAITOSpawn = AIToSpawn;
	NumberOfAISpawn = NumberToSpawn;

	CurrentNumberOfAISpawned = 0;

	SpawnAI();
}


void ASpawner::SpawnAI() {
	AMainAICharacter* NewPawn = GetWorld()->SpawnActor<AMainAICharacter>(CurrentAITOSpawn, GetActorLocation(), GetActorRotation(), ActorSpawnParameters);
	NewPawn->SetWaveManager(WaveManager);
	WaveManager->AddAIToList(NewPawn);
	Gamemode->AddGlitch(NewPawn->GetMainAIController()->GetAIData()->AISpawnGlitchValue);

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

bool ASpawner::AnyAILeftToSpawn() const{
	return CurrentNumberOfAISpawned < NumberOfAISpawn;
}

FStateAtWave ASpawner::GetStateAtWave() const{
	return StateAtWave;
}

void ASpawner::ForceEndSpawn() {
	NumberOfAISpawn = CurrentNumberOfAISpawned;
}