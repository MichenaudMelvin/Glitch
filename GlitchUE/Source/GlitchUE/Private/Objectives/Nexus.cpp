// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Nexus.h"
#include "Kismet/GameplayStatics.h"
#include "GlitchUEGameMode.h"
#include "AI/Waves/WaveManager.h"

ANexus::ANexus() {

}

void ANexus::BeginPlay() {
	Super::BeginPlay();
}

void ANexus::HealthNull(){
	Super::HealthNull();
	UE_LOG(LogTemp, Warning, TEXT("Le nexus n'a plus de vie"));
}

void ANexus::ActiveObjectif(){
	if (ActivableComp->IsActivated()) {
		Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(this))->SetNewPhase(EPhases::TowerDefense);
	}
}

void ANexus::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	if (!ActivableComp->IsActivated()) {
		ActivableComp->ActivateObject();
	}
}
