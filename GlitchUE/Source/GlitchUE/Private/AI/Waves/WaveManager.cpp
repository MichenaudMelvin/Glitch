// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Waves/WaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/Catalyseur.h"
#include "Objectives/Nexus.h"
#include "AI/Waves/Spawner.h"
#include "EngineUtils.h"
#include "Audio/AudioManager.h"
#include "Player/MainPlayerController.h"

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

#if WITH_EDITOR

	if (CatalyseursList.Num() == 0){
		UE_LOG(LogTemp, Fatal, TEXT("AUCUN CATALYSEUR EST PLACE DANS LA SCENE"));
	}

#endif

	TArray<ASpawner*> SpawnerArray;

	FindAllActors<ASpawner>(GetWorld(), SpawnerArray);

	SpawnerList = TSet<ASpawner*>(SpawnerArray);

#if WITH_EDITOR

	if (SpawnerList.Num() == 0){
		UE_LOG(LogTemp, Fatal, TEXT("AUCUN SPAWNER EST PLACE DANS LA SCENE"));
	}

#endif

	TArray<AActor*> NexusArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANexus::StaticClass(), NexusArray);

#if WITH_EDITOR

	if (NexusArray.Num() == 0){
		UE_LOG(LogTemp, Fatal, TEXT("LE NEXUS N'EST PAS PLACE DANS LA SCENE"));
	}

#endif

	Nexus = Cast<ANexus>(NexusArray[0]);

	GameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){
		PlayerTimerWidget = Player->GetMainPlayerController()->GetTimerWidget();
	}, 0.1f, false);

	TArray<AActor*> AudioManagerArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAudioManager::StaticClass(), AudioManagerArray);

	AudioManager = Cast<AAudioManager>(AudioManagerArray[0]);
}

void AWaveManager::EnableSpawners(){
	TArray<ASpawner*> SpawnerArray = SpawnerList.Array();
	for (int i = 0; i < SpawnerArray.Num(); i++){
		if(SpawnerArray[i]->GetActivableComp()->IsActivated()){
			continue;
		}

		if(SpawnerArray[i]->GetStateAtWave().EnableAtWave <= CurrentWaveNumber && SpawnerArray[i]->GetStateAtWave().DisableAtWave >= CurrentWaveNumber){
			SpawnerArray[i]->GetActivableComp()->ActivateObject();
			ActiveSpawnerList.Add(SpawnerArray[i]);
		}
	}
}

void AWaveManager::DisableSpawner(){
	TArray<ASpawner*> SpawnerArray = SpawnerList.Array();
	for (int i = 0; i < SpawnerArray.Num(); i++){
		if (SpawnerArray[i]->GetStateAtWave().DisableAtWave == CurrentWaveNumber){
			SpawnerArray[i]->GetActivableComp()->DesactivateObject();
			ActiveSpawnerList.Remove(SpawnerArray[i]);
		}
	}
}

void AWaveManager::StartWave(){
	GameMode->GlobalWorldSave(0);

	bIsStopped = false;

	const FWave CurrentWave = GetCurrentWaveData();

	OnStartWave.Broadcast(CurrentWaveNumber);

	EnableSpawners();

	SpawnEnemies();
}

void AWaveManager::EndWave(){

	DisableSpawner();

	const FWave CurrentWave = GetCurrentWaveData();

	OnEndWave.Broadcast(CurrentWaveNumber);

	if (CurrentWaveNumber == NumberOfWaves){
		return;
	}

	if (GetCurrentWaveData().bStopAtEnd){
		bIsStopped = true;
		return;
	}

	FKOnFinishTimer NextWaveEvent;
	NextWaveEvent.BindDynamic(this, &AWaveManager::NextWave);
	PlayerTimerWidget->StartTimer(GetCurrentWaveData().NextWaveTimer, NextWaveEvent);
}

void AWaveManager::SpawnEnemies(){
	TArray<FAIToSpawn> ListOfAIToSpawn = GetCurrentWaveData().AIToSpawnList;

#if WITH_EDITOR

	if (ActiveSpawnerList.Num() == 0){
		UE_LOG(LogTemp, Fatal, TEXT("AUCUN SPAWNER EST ACTIF PENDANT LA VAGUE %d"), CurrentWaveNumber);
	}

#endif

	for (int i = 0; i < ListOfAIToSpawn.Num(); i++){
		const int NumberToSpawnForEachSpawners = ListOfAIToSpawn[i].NumberToSpawn / ActiveSpawnerList.Num();
		for (int j = 0; j < ActiveSpawnerList.Num(); j++){
			ActiveSpawnerList[j]->BeginSpawn(NumberToSpawnForEachSpawners, ListOfAIToSpawn[i].AIToSpawn, ListOfAIToSpawn[i].AIData);
		}
	}
}

FWave AWaveManager::GetCurrentWaveData() const{
	return *WavesData->FindRow<FWave>(WavesData->GetRowNames()[CurrentWaveNumber - 1], "");
}

FWave AWaveManager::GetTargetWaveData(const int Target) const{
	return *WavesData->FindRow<FWave>(WavesData->GetRowNames()[Target - 1], "");
}

bool AWaveManager::IsStopped() const{
	return bIsStopped;
}

void AWaveManager::AddAIToList(AMainAICharacter* AIToAdd){
	WaveAIList.Add(AIToAdd);
	OnRefreshAIList.Broadcast();
}

void AWaveManager::RemoveAIFromList(const AMainAICharacter* AIToRemove){
	WaveAIList.Remove(AIToRemove);

	OnRefreshAIList.Broadcast();

	if (WaveAIList.Num() == 0){
		if (HaveTheSpawnerFinished()){
			EndWave();
		}
	}
}

void AWaveManager::NextWave(){
// je le garde pour plutard pour des debug functions
// #if !UE_BUILD_SHIPPING
//
// 	// used for skip wave in debug
// 	if(!bIsStopped){
// 		for (int i = 0; i < ActiveSpawnerList.Num(); i++){
// 			ActiveSpawnerList[i]->ForceEndSpawn();
// 		}
// 	
// 		TArray<AMainAICharacter*> AIList = WaveAIList.Array();
// 	
// 		for (int i = 0; i < AIList.Num(); i++){
// 			AIList[i]->GetHealthComp()->TakeMaxDamages();
// 		}
// 	
// 		return;
// 	}
//
// #endif

	CurrentWaveNumber++;

	AudioManager->UpdateTowerDefenseMusic();
	
	UE_LOG(LogTemp, Warning, TEXT("UpdateTowerDefenseMusic"));

	if(CurrentWaveNumber >= NumberOfWaves){
		GEngine->AddOnScreenDebugMessage(-1, 100000.0f, FColor::Blue, TEXT("Les vagues sont terminées"));
		return;
	}

	SetWave(CurrentWaveNumber);
}

void AWaveManager::SetWave(const int NewWave){
	CurrentWaveNumber = NewWave;

	StartWave();
}

int AWaveManager::GetCurrentWaveNumber() const{
	return CurrentWaveNumber;
}

bool AWaveManager::HaveTheSpawnerFinished(){
	for (int i = 0; i < ActiveSpawnerList.Num(); i++){
		if (ActiveSpawnerList[i]->AnyAILeftToSpawn()){
			return false;
		}
	}

	return true;
}
