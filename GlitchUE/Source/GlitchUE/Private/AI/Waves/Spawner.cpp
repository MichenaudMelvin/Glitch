// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Waves/Spawner.h"
#include "Engine/World.h"
#include "AI/MainAIController.h"
#include "AI/Waves/WaveManager.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"

ASpawner::ASpawner(){
	PrimaryActorTick.bCanEverTick = false;

	SpawnerFX = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("Spawner FX"));
	SetRootComponent(SpawnerFX);

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> FX(TEXT("/Game/VFX/Particles/FX_Enemies/Pk_Spawner"));
	check(FX.Succeeded());

	SpawnerFX->SetEffect(FX.Object);
	SpawnerFX->SetMobility(EComponentMobility::Static);

	ActivableComp = CreateDefaultSubobject<UActivableComponent>(TEXT("Activable Comp"));

	CompassIcon = CreateDefaultSubobject<UCompassIcon>(TEXT("Spawner Compass"));
	CompassIcon->SetAllowDraw(false);

	#if WITH_EDITORONLY_DATA

		Billboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
		Billboard->SetupAttachment(RootComponent);
		Billboard->bIsEditorOnly = true;

	#endif
}

void ASpawner::BeginPlay(){
	Super::BeginPlay();

	TArray<AActor*> WaveManagerTemp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaveManager::StaticClass(), WaveManagerTemp);
	WaveManager = Cast<AWaveManager>(WaveManagerTemp[0]);
	Gamemode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(this));

	ActivableComp->OnActivated.AddDynamic(this, &ASpawner::ActivateSpawner);
	ActivableComp->OnDesactivated.AddDynamic(this, &ASpawner::DesactivateSpawner);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){
		SpawnerFX->StopEmitter();
	}, 0.001f, false);
}

void ASpawner::ActivateSpawner(){
	SpawnerFX->StartEmitter();
	CompassIcon->SetAllowDraw(true);
}

void ASpawner::DesactivateSpawner(){
	SpawnerFX->StopEmitter();
	CompassIcon->SetAllowDraw(false);
}

void ASpawner::BeginSpawn(const int NumberToSpawn, const TSubclassOf<AMainAICharacter> AIToSpawn, UMainAIData* AIData) {
	CurrentAITOSpawn = AIToSpawn;
	NumberOfAISpawn = NumberToSpawn;
	CurrentAIData = AIData;

	CurrentNumberOfAISpawned = 0;

	SpawnAI();
}


void ASpawner::SpawnAI() {
	AMainAICharacter* NewPawn = GetWorld()->SpawnActor<AMainAICharacter>(CurrentAITOSpawn, GetActorLocation(), GetActorRotation(), ActorSpawnParameters);

	if(!IsValid(NewPawn)){
		UE_LOG(LogTemp, Warning, TEXT("Spawner try to instantiate a pawn"));
		return;
	}

	NewPawn->SetWaveManager(WaveManager);
	WaveManager->AddAIToList(NewPawn);

#if WITH_EDITOR
	if(!IsValid(CurrentAIData)){
		UE_LOG(LogTemp, Fatal, TEXT("LES DATA DE LA VAGUE %d SONT NULLES"), WaveManager->GetCurrentWaveNumber());
	}
#endif

	NewPawn->GetMainAIController()->SetCurrentData(CurrentAIData);

	Gamemode->AddGlitch(CurrentAIData->AISpawnGlitchValue);

	CurrentNumberOfAISpawned++;

	if (AnyAILeftToSpawn()) {
		FTimerHandle TimerHandle;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASpawner::SpawnAI, SpawnDelay, false);
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