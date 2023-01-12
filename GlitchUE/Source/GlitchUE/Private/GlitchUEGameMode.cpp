// Copyright Epic Games, Inc. All Rights Reserved.

#include "GlitchUEGameMode.h"
#include "Player/MainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AGlitchUEGameMode::AGlitchUEGameMode(){
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/Player/BP_MainPlayer"));
	// if (PlayerPawnBPClass.Class != NULL)
	// {
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }
}

void AGlitchUEGameMode::BeginPlay() {
	//set player
	// set wave manager
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
		//WaveManager->StartWave();
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
		OnGlitchMax.Broadcast();
		GlitchValue = 0;
	}
}

void AGlitchUEGameMode::SetGlobalTimeDilation(float TimeDilation){
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
}
