// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Waves/WaveManager.h"
#include "Kismet/GameplayStatics.h"

AWaveManager::AWaveManager(){
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent->SetMobility(EComponentMobility::Static);

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/Blueprint/AI/AISpawner/DT_Waves"));
	check(DataTable.Succeeded());

	WavesData = DataTable.Object;

}

void AWaveManager::BeginPlay(){
	Super::BeginPlay();
	Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));	

	NumberOfWaves = WavesData->GetRowNames().Num();

	TArray<AActor*> CatalyseurArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACatalyseur::StaticClass(), CatalyseurArray);
	CatalyseursList = TSet<ACatalyseur*>(CatalyseursList);
}

void AWaveManager::EnableCatalyseurs(){
	TArray<ACatalyseur*> CatalyseurArray = CatalyseursList.Array();
	for (int i = 0; i < CatalyseurArray.Num(); i++) {
		if (CatalyseurArray[i]->GetStateAtWave().EnableAtWave == CurrentWaveNumber+1) {
			CatalyseurArray[i]->GetActivableComp()->ActivateObject();
		}
	}
}

void AWaveManager::DisableCatalyseurs() {
	TArray<ACatalyseur*> CatalyseurArray = CatalyseursList.Array();
	for (int i = 0; i < CatalyseurArray.Num(); i++) {
		if (CatalyseurArray[i]->GetStateAtWave().DisableAtWave == CurrentWaveNumber + 1) {
			CatalyseurArray[i]->GetActivableComp()->DesactivateObject();
		}
	}
}

void AWaveManager::StartWave(){
	UE_LOG(LogTemp, Warning, TEXT("StartWave"));

	FWave* CurrentWave = GetCurrentWaveData();
	if (CurrentWave->GivenGolds.WaveEvent == EWaveEvent::ExecuteAtStart) {
		Player->GiveGolds(CurrentWave->GivenGolds.Golds);
	}

	EnableCatalyseurs();

	SpawnEnemies();

	if (CurrentWaveNumber == NumberOfWaves) {
		return;
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		EndWave();
	}, GetCurrentWaveData()->WaveDuration, false);
}

void AWaveManager::EndWave() {
	UE_LOG(LogTemp, Warning, TEXT("EndWave"));

	DisableCatalyseurs();

	FWave* CurrentWave = GetCurrentWaveData();

	if (CurrentWave->GivenGolds.WaveEvent == EWaveEvent::ExecuteAtStart) {
		Player->GiveGolds(CurrentWave->GivenGolds.Golds);
	}

	CurrentWaveNumber++;
	StartWave();
}

void AWaveManager::SpawnEnemies(){
	TArray<FAIToSpawn> ListOfAIToSpawn = GetCurrentWaveData()->AIToSpawnList;

	for (int i = 0; i < ListOfAIToSpawn.Num(); i++) {
		int NumberToSpawnForEachSpawners = ListOfAIToSpawn[i].NumberToSpawn / ActiveSpawnerList.Num();
		for (int j = 0; j < ActiveSpawnerList.Num(); j++) {
			ActiveSpawnerList[j]->Spawn(NumberToSpawnForEachSpawners, ListOfAIToSpawn[i].AIToSpawn);
		}
	}

}

FWave* AWaveManager::GetCurrentWaveData() {
	return WavesData->FindRow<FWave>(WavesData->GetRowNames()[CurrentWaveNumber], "");
}

void AWaveManager::RefreshActiveSpawners() {
	ActiveSpawnerList.Empty();
	TArray<ASpawner*> SpawnerArray = SpawnerList.Array();

	for (int i = 0; i < SpawnerArray.Num(); i++) {
		if (SpawnerArray[i]->GetActivableComp()->GetState() == EState::CPF_Activated) {
			ActiveSpawnerList.Add(SpawnerArray[i]);
		}
	}
}