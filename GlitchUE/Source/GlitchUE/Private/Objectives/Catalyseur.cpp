// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Catalyseur.h"
#include "AI/Waves/Spawner.h"

void ACatalyseur::BeginPlay() {
	Super::BeginPlay();

	if (!IsValid(Nexus)) {
		UE_LOG(LogTemp, Fatal, TEXT("LE CATALYSEUR %s N'A PAS DE NEXUS"), *this->GetName());
	}
}

void ACatalyseur::ActiveObjectif(){
	if (Nexus->GetActivableComp()->GetState() == EState::CPF_Activated) {
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