// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Catalyseur.h"

void ACatalyseur::BeginPlay() {

}

void ACatalyseur::ActiveObjectif(){
	if (Nexus->GetActivableComp()->GetState() == EState::CPF_Activated && ActivableComp->GetState() == EState::CPF_Desactivated) {
		//for (int i = 0; i < sizeof(ConstructionZoneList); i++) {
			//ConstructionZoneList[i]->GetActivableComp()->ActivateObject();
			
		//}

		//for (int i = 0; i < sizeof(SpawnList); i++) {
			//SpawnList[i]->GetActivableComp()->ActivateObject();

		//}
	}
}

void ACatalyseur::DesactivateObjectif() {
	//for (int i = 0; i < sizeof(ConstructionZoneList); i++) {
	//ConstructionZoneList[i]->GetActivableComp()->DesactivateObject();

	//}

	//for (int i = 0; i < sizeof(SpawnList); i++) {
		//SpawnList[i]->GetActivableComp()->DesactivateObject();

	//}
}

void ACatalyseur::OnHealthNull(){
	ActivableComp->DesactivateObject();
}
