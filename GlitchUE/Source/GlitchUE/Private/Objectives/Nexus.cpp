// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Nexus.h"

ANexus::ANexus() {
	Interaction = CreateDefaultSubobject<UInteractableComponent>(TEXT("NexusInteraction"));
}


void ANexus::BeginPlay() {
	Super::BeginPlay();
	Interaction->AddInteractable(MeshObjectif);
}

void ANexus::ActiveObjectif(){
	if (ActivableComp->GetState() == EState::CPF_Activated) {
		ActivableComp->ActivateObject();
		//set gamemode phase
	}
}