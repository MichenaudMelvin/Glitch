// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Waves/WaveManager.h"
#include "Kismet/GameplayStatics.h"

AWaveManager::AWaveManager(){
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/Blueprint/AI/AISpawner/DT_Waves"));
	check(DataTable.Succeeded());

	WavesData = DataTable.Object;

}

void AWaveManager::BeginPlay(){
	Super::BeginPlay();
	Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));	

	NumberOfWaves = WavesData->GetRowNames().Num();
	
}

void AWaveManager::StartWave(){
	FWave* CurrentWave = GetCurrentWaveData();
	if (CurrentWave->GivenGolds.WaveEvent == EWaveEvent::ExecuteAtStart) {
		Player->GiveGolds(CurrentWave->GivenGolds.Golds);
	}

	//SpawnEnemies();

	if (CurrentWaveNumber == NumberOfWaves) {
		return;
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		EndWave();
	}, GetCurrentWaveData()->WaveDuration, false);
}

void AWaveManager::EndWave() {
	FWave* CurrentWave = GetCurrentWaveData();

	if (CurrentWave->GivenGolds.WaveEvent == EWaveEvent::ExecuteAtStart) {
		Player->GiveGolds(CurrentWave->GivenGolds.Golds);
	}

	CurrentWaveNumber++;
	StartWave();
}

FWave* AWaveManager::GetCurrentWaveData() {
	return WavesData->FindRow<FWave>(WavesData->GetRowNames()[CurrentWaveNumber], "");
}