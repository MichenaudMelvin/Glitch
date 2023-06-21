// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Waves/WaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/Catalyseur.h"
#include "Objectives/Nexus.h"
#include "AI/Waves/Spawner.h"
#include "EngineUtils.h"
#include "AI/AIFocusCatalyseur/FocusCatalyseurCharacter.h"
#include "Audio/AudioManager.h"
#include "UI/Gameplay/PlayerStats.h"
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

	TArray<AActor*> AudioManagerArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAudioManager::StaticClass(), AudioManagerArray);

	AudioManager = Cast<AAudioManager>(AudioManagerArray[0]);

#if WITH_EDITOR
	// for simulation mode 
	if(!IsValid(Player)){
		return;
	}
#endif

	FWorldDelegates::OnWorldCleanup.AddUFunction(this, "OnCleanWorld");

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){
		const AMainPlayerController* PlayerController = Player->GetMainPlayerController();

		PlayerTimerWidget = PlayerController->GetTimerWidget();
		PlayerMessageWidget = PlayerController->GetAdditionalMessageWidget();
		PlayerTchatWidget = PlayerController->GetTchatWidget();
		PlayerStatsWidget = PlayerController->GetPlayerStatsWidget();
	}, 0.1f, false);
}

void AWaveManager::OnCleanWorld(UWorld* World, bool bSessionEnded, bool bCleanupResources){
	World->GetTimerManager().ClearTimer(ObjectiveDelayTimerHandle);
	World->GetTimerManager().ClearTimer(MessageDelayTimerHandle);
	World->GetTimerManager().ClearTimer(PrepareTimerHandle);
}

void AWaveManager::UpdatePlayerObjectives(){
	if(!IsValid(PlayerMessageWidget) || !IsValid(PlayerStatsWidget)){
		// only useful on load save

		GetWorld()->GetTimerManager().SetTimer(ObjectiveDelayTimerHandle, this, &AWaveManager::UpdatePlayerObjectives, 0.1f, false);
		return;
	}

	PlayerMessageWidget->AddMessageToScreen("Starting Wave: " + FString::FromInt(CurrentWaveNumber));

	if(!GameMode->UseAutoObjectivesForPlayer()){
		return;
	}

	PlayerStatsWidget->UpdateObjectivesText("Current Wave: " + FString::FromInt(CurrentWaveNumber));
	PlayerStatsWidget->UpdateAdditionalText("");
}

void AWaveManager::WriteWhatTheNextWaveContain(const FWave TargetWave, const int TargetWaveIndex){
	PlayerTchatWidget->AddEmptyTchatLine();
	PlayerTchatWidget->AddTchatLine("I.V.A.N.", "The next wave contains:", TchatSpeakerColor);

	TchatTargetWave = TargetWave;
	TchatIndex = GetActiveSpawnersAtWave(TargetWaveIndex);

	GetWorld()->GetTimerManager().SetTimer(MessageDelayTimerHandle, this, &AWaveManager::WriteMessages, MessagesDelay, false);
}

void AWaveManager::WriteMessages(){
	TArray<FTchatStruct> MessageList;

	for(int i = 0; i < TchatTargetWave.AIToSpawnList.Num(); i ++){
		const FString DroneType = TchatTargetWave.AIToSpawnList[i].AIToSpawn.GetDefaultObject()->IsA(AFocusCatalyseurCharacter::StaticClass()) ? "Focus Catalyser" : "Focus Nexus";
		const FString Message = FString::FromInt(TchatTargetWave.AIToSpawnList[i].NumberToSpawn * TchatIndex) + " " + DroneType + " " + TchatTargetWave.AIToSpawnList[i].AIData->DroneName;

		MessageList.Add(FTchatStruct(Speaker, Message, TchatSpeakerColor, MessagesDelay));
	}

	PlayerTchatWidget->AddMultipleTchatLines(MessageList);
}

void AWaveManager::EnableSpawners(const int TargetWave){
	TArray<ASpawner*> SpawnerArray = SpawnerList.Array();
	for (int i = 0; i < SpawnerArray.Num(); i++){
		if(SpawnerArray[i]->GetActivableComp()->IsActivated()){
			continue;
		}

		if(SpawnerArray[i]->GetStateAtWave().EnableAtWave <= TargetWave && SpawnerArray[i]->GetStateAtWave().DisableAtWave >= TargetWave){
			SpawnerArray[i]->GetActivableComp()->ActivateObject();
			ActiveSpawnerList.Add(SpawnerArray[i]);
		}
	}
}

void AWaveManager::DisableSpawner(const int TargetWave){
	TArray<ASpawner*> SpawnerArray = SpawnerList.Array();
	for (int i = 0; i < SpawnerArray.Num(); i++){
		if(!SpawnerArray[i]->GetActivableComp()->IsActivated()){
			continue;
		}

		if (SpawnerArray[i]->GetStateAtWave().DisableAtWave == TargetWave){
			SpawnerArray[i]->GetActivableComp()->DesactivateObject();
			ActiveSpawnerList.Remove(SpawnerArray[i]);
		}
	}
}

int AWaveManager::GetActiveSpawnersAtWave(const int TargetWave) const{
	TArray<ASpawner*> SpawnerArray = SpawnerList.Array();

	int ActiveSpawnerNumber = 0;

	for (int i = 0; i < SpawnerArray.Num(); i++){
		if(SpawnerArray[i]->GetStateAtWave().EnableAtWave <= TargetWave && SpawnerArray[i]->GetStateAtWave().DisableAtWave >= TargetWave){
			ActiveSpawnerNumber++;
		}
	}

	return ActiveSpawnerNumber;
}

void AWaveManager::StartPrepareTimer(){
	WriteWhatTheNextWaveContain(GetCurrentWaveData(), CurrentWaveNumber);

	FKOnFinishTimer FinishEvent;
	FinishEvent.BindDynamic(this, &AWaveManager::StartWave);
	PlayerTimerWidget->StartTimer(PrepareTime, FinishEvent);

	AudioManager->SwitchToPauseMusic();

	EnableSpawners(CurrentWaveNumber);

	GetWorld()->GetTimerManager().SetTimer(PrepareTimerHandle, AudioManager, &AAudioManager::SwitchToTowerDefenseMusic, PrepareTime - AudioManager->GetFadeDuration(), false);

	if(!GameMode->UseAutoObjectivesForPlayer()){
		return;
	}

	PlayerStatsWidget->UpdateObjectivesText(PrepareObjectiveText);
	PlayerStatsWidget->UpdateAdditionalText(PrepareAdditionalText);
}

void AWaveManager::StartWave(){
	GameMode->GlobalWorldSave(0);

	bIsStopped = false;

	const FWave CurrentWave = GetCurrentWaveData();

	OnStartWave.Broadcast(CurrentWaveNumber);

	UpdatePlayerObjectives();

	SpawnEnemies();
}

void AWaveManager::EndWave(){

	EnableSpawners(CurrentWaveNumber + 1);

	DisableSpawner(CurrentWaveNumber + 1);

	const FWave CurrentWave = GetCurrentWaveData();

	OnEndWave.Broadcast(CurrentWaveNumber);

	if(CurrentWaveNumber == NumberOfWaves){
		OnFinishAllWaves.Broadcast();
		PlayerMessageWidget->AddMessageToScreen("Finish all Waves");
		return;
	}

	AudioManager->SwitchToPauseMusic();

	PlayerMessageWidget->AddMessageToScreen("Finish Wave: " + FString::FromInt(CurrentWaveNumber));
	WriteWhatTheNextWaveContain(GetNextWaveData(), CurrentWaveNumber + 1);

	if(GetCurrentWaveData().bStopAtEnd){
		bIsStopped = true;
		return;
	}

	FKOnFinishTimer NextWaveEvent;
	NextWaveEvent.BindDynamic(this, &AWaveManager::NextWave);
	PlayerTimerWidget->StartTimer(GetCurrentWaveData().NextWaveTimer, NextWaveEvent);
}

void AWaveManager::SpawnEnemies(){
	TArray<FAIToSpawn> ListOfAIToSpawn = GetCurrentWaveData().AIToSpawnList;

	if (GameMode->OptionsString != ""){
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
			EnableSpawners(CurrentWaveNumber);
			SpawnEnemies();
		}, 0.2f, false);
		return;
	}


	if (ActiveSpawnerList.Num() == 0){

#if WITH_EDITOR
		UE_LOG(LogTemp, Fatal, TEXT("AUCUN SPAWNER EST ACTIF PENDANT LA VAGUE %d"), CurrentWaveNumber);
#endif

		// this is not supposed to happen but it's a security
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
			EnableSpawners(CurrentWaveNumber);
			SpawnEnemies();
		}, 0.2f, false);
		return;
	}


	for (int i = 0; i < ListOfAIToSpawn.Num(); i++){
		for (int j = 0; j < ActiveSpawnerList.Num(); j++){
			ActiveSpawnerList[j]->BeginSpawn(ListOfAIToSpawn[i].NumberToSpawn, ListOfAIToSpawn[i].AIToSpawn, ListOfAIToSpawn[i].AIData);
		}
	}
}

FWave AWaveManager::GetCurrentWaveData() const{
	return *WavesData->FindRow<FWave>(WavesData->GetRowNames()[CurrentWaveNumber - 1], "");
}

FWave AWaveManager::GetNextWaveData() const{
	return *WavesData->FindRow<FWave>(WavesData->GetRowNames()[CurrentWaveNumber], "");
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
	CurrentWaveNumber++;

	AudioManager->SwitchToTowerDefenseMusic();

	SetWave(CurrentWaveNumber);
}

void AWaveManager::ForceNextWave(){
#if !UE_BUILD_SHIPPING
	// used for skip wave in debug
	if(!bIsStopped){
		for (int i = 0; i < ActiveSpawnerList.Num(); i++){
			ActiveSpawnerList[i]->ForceEndSpawn();
		}

		TArray<AMainAICharacter*> AIList = WaveAIList.Array();

		for (int i = 0; i < AIList.Num(); i++){
			AIList[i]->GetHealthComp()->TakeMaxDamages();
		}

		//NextWave();
	}
#endif
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
