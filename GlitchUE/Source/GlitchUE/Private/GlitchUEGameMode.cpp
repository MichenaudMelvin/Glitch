// Copyright Epic Games, Inc. All Rights Reserved.

#include "GlitchUEGameMode.h"
#include "Player/MainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "PlacableObject/PlacableActor.h"
#include "UObject/ConstructorHelpers.h"
#include "AI/Waves/WaveManager.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"

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

		EGlitchEvent::Type RandomGlitchType = static_cast<EGlitchEvent::Type>(FMath::RandRange(0, 3));

		switch (RandomGlitchType){
		case EGlitchEvent::UpgradeAlliesUnits:
			UE_LOG(LogTemp, Warning, TEXT("UpgradeAlliesUnits"));
			GlitchUpgradeAlliesUnits();
			break;
		
		case EGlitchEvent::UpgradeEnemiesAI:
			UE_LOG(LogTemp, Warning, TEXT("UpgradeEnemiesAI"));
			GlitchUpgradeEnemiesAI();
			break;
		
		case EGlitchEvent::UpgradePlayer:
			UE_LOG(LogTemp, Warning, TEXT("UpgradePlayer"));
			GlitchUpgradePlayer();
			break;
		
		case EGlitchEvent::RandomFX:
			UE_LOG(LogTemp, Warning, TEXT("RandomFX"));
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
	TArray<AActor*> PlacableActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlacableActor::StaticClass(), PlacableActorList);

	PlacableActorList = UUsefullFunctions::SortActorsByDistanceToActor(PlacableActorList, MainPlayer);
	Cast<APlacableActor>(PlacableActorList[0])->GlitchUpgrade();
}

void AGlitchUEGameMode::GlitchUpgradeEnemiesAI(){

}

void AGlitchUEGameMode::GlitchUpgradePlayer() {

}

void AGlitchUEGameMode::GlitchRandomFX() {

}

#pragma region ConsoleCommands

void AGlitchUEGameMode::SetGlobalTimeDilation(float TimeDilation){
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
}

#pragma endregion