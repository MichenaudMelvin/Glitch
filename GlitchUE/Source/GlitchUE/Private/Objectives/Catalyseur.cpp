// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Catalyseur.h"
#include "AI/Waves/Spawner.h"

void ACatalyseur::BeginPlay() {

}

void ACatalyseur::ActiveObjectif(){
	if (Nexus->GetActivableComp()->GetState() == EState::CPF_Activated && ActivableComp->GetState() == EState::CPF_Desactivated) {
		for (int i = 0; i < ConstructionZoneList.Num(); i++) {
			ConstructionZoneList[i]->GetActivableComp()->ActivateObject();
		}

		for (int i = 0; i < SpawnerList.Num(); i++) {
			SpawnerList[i]->GetActivableComp()->ActivateObject();
		}
	}
}

void ACatalyseur::DesactivateObjectif() {
	for (int i = 0; i < ConstructionZoneList.Num(); i++) {
		ConstructionZoneList[i]->GetActivableComp()->DesactivateObject();
	}

	for (int i = 0; i < SpawnerList.Num(); i++){
		SpawnerList[i]->GetActivableComp()->DesactivateObject();
	}
}

void ACatalyseur::OnHealthNull(){
	ActivableComp->DesactivateObject();
}

FStateAtWave ACatalyseur::GetStateAtWave(){
	return StateAtWave;
}
