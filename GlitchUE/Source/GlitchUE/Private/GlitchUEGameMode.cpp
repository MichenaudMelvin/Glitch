// Copyright Epic Games, Inc. All Rights Reserved.

#include "GlitchUEGameMode.h"
#include "Player/MainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "PlacableObject/PlacableActor.h"
#include "UObject/ConstructorHelpers.h"
#include "AI/Waves/WaveManager.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "AI/MainAICharacter.h"

AGlitchUEGameMode::AGlitchUEGameMode(){
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/Player/BP_MainPlayer"));
	// if (PlayerPawnBPClass.Class != NULL)
	// {
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }
}

void AGlitchUEGameMode::BeginPlay() {
	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	TArray<AWaveManager*> WaveManagerArray;
	FindAllActors<AWaveManager>(GetWorld(), WaveManagerArray);
	if (WaveManagerArray.Num() == 0) {
		UE_LOG(LogTemp, Fatal, TEXT("AUCUN WAVE MANAGER N'EST PLACE DANS LA SCENE"));
	}

	WaveManager = WaveManagerArray[0];
}

EPhases AGlitchUEGameMode::GetPhases(){
	return CurrentPhase;
}

void AGlitchUEGameMode::SetNewPhase(EPhases NewPhase){
	CurrentPhase = NewPhase;
	switch (CurrentPhase){
	case EPhases::Infiltration:
		break;
	case EPhases::TowerDefense:
		WaveManager->StartWave();
		break;
	}
}

ELevelState AGlitchUEGameMode::GetLevelState(){
	return LevelState;
}

void AGlitchUEGameMode::SetLevelState(ELevelState newState){
	LevelState = newState;
}

void AGlitchUEGameMode::AddGlitch(float AddedValue){
	GlitchValue = FMath::Clamp(AddedValue + GlitchValue, 0.0f, GlitchMaxValue);
	if (GlitchValue == GlitchMaxValue) {

		//OnGlitchMax.Broadcast();
		CheckAvailableGlitchEvents();
		EGlitchEvent::Type RandomGlitchType;

		while (true){
			RandomGlitchType = static_cast<EGlitchEvent::Type>(FMath::RandRange(0, 3));
		}

		switch (RandomGlitchType){
		case EGlitchEvent::UpgradeAlliesUnits:
			GlitchUpgradeAlliesUnits();
			break;
		
		case EGlitchEvent::UpgradeEnemiesAI:
			GlitchUpgradeEnemiesAI();
			break;
		
		case EGlitchEvent::UpgradePlayer:
			GlitchUpgradePlayer();
			break;
		
		case EGlitchEvent::RandomFX:
			GlitchRandomFX();
			break;
		}

		GlitchValue = 0;
	}
}

float AGlitchUEGameMode::GetCurrentGlitchValue(){
	return GlitchValue;
}

void AGlitchUEGameMode::GlitchUpgradeAlliesUnits(){
	UE_LOG(LogTemp, Warning, TEXT("UpgradeAlliesUnits"));

	TArray<AActor*> PlacableActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlacableActor::StaticClass(), PlacableActorList);

	if (PlacableActorList.Num() > 0) {
		PlacableActorList = UUsefullFunctions::SortActorsByDistanceToActor(PlacableActorList, MainPlayer);
		Cast<APlacableActor>(PlacableActorList[0])->GlitchUpgrade();
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Aucune tourlles dans le monde"));
	}
}

void AGlitchUEGameMode::GlitchUpgradeEnemiesAI(){
	UE_LOG(LogTemp, Warning, TEXT("UpgradeEnemiesAI"));
	
	TArray<AActor*> AIList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainAICharacter::StaticClass(), AIList);

	if (AIList.Num() > 0) {
		AIList = UUsefullFunctions::SortActorsByDistanceToActor(AIList, MainPlayer);
		Cast<AMainAICharacter>(AIList[0])->GlitchUpgrade();
	} else{
		UE_LOG(LogTemp, Warning, TEXT("Aucun ennemis dans le monde"));
	}

}

void AGlitchUEGameMode::GlitchUpgradePlayer() {
	UE_LOG(LogTemp, Warning, TEXT("UpgradePlayer"));

	MainPlayer->GlitchUpgrade();
}

void AGlitchUEGameMode::GlitchRandomFX() {
	UE_LOG(LogTemp, Warning, TEXT("RandomFX"));
}

void AGlitchUEGameMode::CheckAvailableGlitchEvents(){
	TArray<AActor*> PlacableActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlacableActor::StaticClass(), PlacableActorList);

	TArray<AActor*> AIList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainAICharacter::StaticClass(), AIList);

}

#pragma region ConsoleCommands

void AGlitchUEGameMode::SetGlobalTimeDilation(float TimeDilation){
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
}

void AGlitchUEGameMode::NextWave(){
	WaveManager->SetWave(WaveManager->GetCurrentWaveNumber() + 1);
}

void AGlitchUEGameMode::GoToWave(int NewWave){
	WaveManager->SetWave(NewWave);
}

#pragma endregion