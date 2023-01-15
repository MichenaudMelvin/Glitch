// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Waves/WaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/Catalyseur.h"
#include "AI/Waves/Spawner.h"
#include "EngineUtils.h"

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

	TArray<ACatalyseur*> CatalyseurArray;	
	FindAllActors<ACatalyseur>(GetWorld(), CatalyseurArray);
	
	CatalyseursList = TSet<ACatalyseur*>(CatalyseurArray);

	if (CatalyseursList.Num() == 0) {
		UE_LOG(LogTemp, Fatal, TEXT("LA LISTE DES CATALYSEURS DU WAVE MANAGER EST VIDE"));
	}

	if (SpawnerList.Num() == 0) {
		UE_LOG(LogTemp, Fatal, TEXT("LA LISTE DES SPAWNER DU WAVE MANAGER EST VIDE"));
	}
}

void AWaveManager::EnableCatalyseurs(){
	TArray<ACatalyseur*> CatalyseurArray = CatalyseursList.Array();

	for (int i = 0; i < CatalyseurArray.Num(); i++) {
		if (CatalyseurArray[i]->GetStateAtWave().EnableAtWave == CurrentWaveNumber+1) {
			CatalyseurArray[i]->GetActivableComp()->ActivateObject();
		}
	}

	RefreshActiveSpawners();
}

void AWaveManager::DisableCatalyseurs() {
	TArray<ACatalyseur*> CatalyseurArray = CatalyseursList.Array();
	for (int i = 0; i < CatalyseurArray.Num(); i++) {
		if (CatalyseurArray[i]->GetStateAtWave().DisableAtWave == CurrentWaveNumber + 1) {
			CatalyseurArray[i]->GetActivableComp()->DesactivateObject();
		}
	}

	RefreshActiveSpawners();
}

void AWaveManager::StartWave() {

	FWave* CurrentWave = GetCurrentWaveData();
	if (CurrentWave->GivenGolds.WaveEvent == EWaveEvent::ExecuteAtStart) {
		Player->GiveGolds(CurrentWave->GivenGolds.Golds);
	}

	EnableCatalyseurs();

	SpawnEnemies();
}

void AWaveManager::EndWave() {

	DisableCatalyseurs();

	FWave* CurrentWave = GetCurrentWaveData();

	if (CurrentWaveNumber == NumberOfWaves) {
		return;
	}

	if (CurrentWave->GivenGolds.WaveEvent == EWaveEvent::ExecuteAtStart) {
		Player->GiveGolds(CurrentWave->GivenGolds.Golds);
	}

	if (GetCurrentWaveData()->bStopAtEnd) {
		CurrentWaveNumber++;
		return;
	}

	CurrentWaveNumber++;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		StartWave();
	}, GetCurrentWaveData()->NextWaveTimer, false);
}

void AWaveManager::SpawnEnemies(){
	TArray<FAIToSpawn> ListOfAIToSpawn = GetCurrentWaveData()->AIToSpawnList;

	if (ActiveSpawnerList.Num() == 0) {
		UE_LOG(LogTemp, Fatal, TEXT("AUCUN SPAWNER EST ACTIF PENDANT LA VAGUE %d"), CurrentWaveNumber+1);
	}

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

void AWaveManager::AddAIToList(AMainAICharacter* AIToAdd) {
	WaveAIList.Add(AIToAdd);
}

void AWaveManager::RemoveAIFromList(AMainAICharacter* AIToRemove){
	WaveAIList.Remove(AIToRemove);
	if (WaveAIList.Num() == 0) {
		EndWave();
	}
}
