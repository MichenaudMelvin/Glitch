// Fill out your copyright notice in the Description page of Project Settings.


#include "GlitchZones/CatalyseurZone.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayer.h"
#include "Player/MainPlayerController.h"

ACatalyseurZone::ACatalyseurZone(){
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/VFX/Particles/Meshes/SM_Demi_Sphere"));
	check(Mesh.Succeeded());

	GetStaticMeshComponent()->SetStaticMesh(Mesh.Object);
}

void ACatalyseurZone::BeginPlay(){
	Super::BeginPlay();

	GameMode->OnSwitchPhases.AddDynamic(this, &ACatalyseurZone::OnSwitchPhases);
}

void ACatalyseurZone::OnPlayerEnterZone(){
	Super::OnPlayerEnterZone();

	if(!IsValid(MainPlayer)){
		return;
	}

	MainPlayer->EnableSafeEffect(true, PostProcessFadeTime);

	MainPlayer->GetMainPlayerController()->GetTimerWidget()->PauseTimer(true);

	if(GameMode->GetLevelState() == ELevelState::Alerted){
		GetWorld()->GetTimerManager().SetTimer(ZoneTimer, [&]() {
			GameMode->SetLevelState(ELevelState::Normal);
		}, ResetLevelStateDuration, false);
	}
}

void ACatalyseurZone::OnPlayerExitZone(){
	Super::OnPlayerExitZone();

	MainPlayer->EnableSafeEffect(false, PostProcessFadeTime);

	MainPlayer->GetMainPlayerController()->GetTimerWidget()->PauseTimer(false);
}

void ACatalyseurZone::OnSwitchPhases(EPhases NewPhase){
	switch (NewPhase){
		case EPhases::Infiltration:
			break;
		case EPhases::TowerDefense:
			Destroy();
			break;
	}
}
