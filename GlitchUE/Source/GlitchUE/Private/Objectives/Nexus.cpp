// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Nexus.h"
#include "Kismet/GameplayStatics.h"
#include "GlitchUEGameMode.h"
#include "AI/Waves/WaveManager.h"

ANexus::ANexus() {

}

void ANexus::BeginPlay() {
	Super::BeginPlay();
	InteractableComp->AddInteractable(MeshObjectif);
}

void ANexus::ActiveObjectif(){
	if (ActivableComp->GetState() == EState::CPF_Activated) {
		Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(this))->SetNewPhase(EPhases::TowerDefense);
	}
}

void ANexus::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	if (ActivableComp->GetState() == EState::CPF_Desactivated) {
		ActivableComp->ActivateObject();
	}
}
