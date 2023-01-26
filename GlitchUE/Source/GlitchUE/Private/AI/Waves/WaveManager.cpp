// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Waves/WaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/Catalyseur.h"
#include "Objectives/Nexus.h"
#include "AI/Waves/Spawner.h"
#include "EngineUtils.h"

AWaveManager::AWaveManager(){
	PrimaryActorTick.bCanEverTick = false;

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
		UE_LOG(LogTemp, Fatal, TEXT("AUCUN CATALYSEUR EST PLACE DANS LA SCENE"));
	}

	TArray<ASpawner*> SpawnerArray;

	FindAllActors<ASpawner>(GetWorld(), SpawnerArray);

	SpawnerList = TSet<ASpawner*>(SpawnerArray);

	if (SpawnerList.Num() == 0) {
		UE_LOG(LogTemp, Fatal, TEXT("AUCUN SPAWNER EST PLACE DANS LA SCENE"));
	}

	TArray<AActor*> NexusArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANexus::StaticClass(), NexusArray);
	
	if (NexusArray.Num() == 0){
		UE_LOG(LogTemp, Fatal, TEXT("LE NEXUS N'EST PAS PLACE DANS LA SCENE"));
	}
	
	Nexus = Cast<ANexus>(NexusArray[0]);

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

void AWaveManager::StartWave_Implementation() {

	FWave CurrentWave = GetCurrentWaveData();
	if (CurrentWave.GivenGolds.WaveEvent == EWaveEvent::ExecuteAtStart) {
		Player->GiveGolds(CurrentWave.GivenGolds.Golds);
	}

	EnableCatalyseurs();

	SpawnEnemies();
}

void AWaveManager::EndWave_Implementation() {

	DisableCatalyseurs();

	FWave CurrentWave = GetCurrentWaveData();

	if (CurrentWaveNumber == NumberOfWaves) {
		return;
	}

	if (CurrentWave.GivenGolds.WaveEvent == EWaveEvent::ExecuteAtStart) {
		Player->GiveGolds(CurrentWave.GivenGolds.Golds);
	}

	if (GetCurrentWaveData().bStopAtEnd) {
		CurrentWaveNumber++;
		return;
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		CurrentWaveNumber++;
		StartWave();
	}, GetCurrentWaveData().NextWaveTimer, false);
}

void AWaveManager::SpawnEnemies(){
	TArray<FAIToSpawn> ListOfAIToSpawn = GetCurrentWaveData().AIToSpawnList;

	if (ActiveSpawnerList.Num() == 0) {
		UE_LOG(LogTemp, Fatal, TEXT("AUCUN SPAWNER EST ACTIF PENDANT LA VAGUE %d"), CurrentWaveNumber+1);
	}

	for (int i = 0; i < ListOfAIToSpawn.Num(); i++) {
		int NumberToSpawnForEachSpawners = ListOfAIToSpawn[i].NumberToSpawn / ActiveSpawnerList.Num();
		for (int j = 0; j < ActiveSpawnerList.Num(); j++) {
			ActiveSpawnerList[j]->BeginSpawn(NumberToSpawnForEachSpawners, ListOfAIToSpawn[i].AIToSpawn);
		}
	}
}

FWave AWaveManager::GetCurrentWaveData() {
	return *WavesData->FindRow<FWave>(WavesData->GetRowNames()[CurrentWaveNumber], "");
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
	OnRefreshAIList.Broadcast();
}

void AWaveManager::RemoveAIFromList(AMainAICharacter* AIToRemove){
	WaveAIList.Remove(AIToRemove);

	OnRefreshAIList.Broadcast();

	if (WaveAIList.Num() == 0) {
		if (HaveTheSpawnerFinished()) {
			EndWave();
		}
	}
}

void AWaveManager::SetWave(int NewWave){
	CurrentWaveNumber = NewWave - 1;

	for (int i = 0; i < ActiveSpawnerList.Num(); i++) {
		ActiveSpawnerList[i]->ForceEndSpawn();
	}

	TArray<AMainAICharacter*> AIList = WaveAIList.Array();

	for (int i = 0; i < AIList.Num(); i++) {
		AIList[i]->GetHealthComp()->TakeMaxDamages();
	}
}

int AWaveManager::GetCurrentWaveNumber(){
	return CurrentWaveNumber;
}

bool AWaveManager::HaveTheSpawnerFinished() {
	for (int i = 0; i < ActiveSpawnerList.Num(); i++) {
		if (ActiveSpawnerList[i]->AnyAILeftToSpawn()) {
			return false;
		}
	}

	return true;
}
