// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Inhibiteur.h"
#include "GlitchUEGameMode.h"
#include "Kismet/GameplayStatics.h"

AInhibiteur::AInhibiteur(){

}

void AInhibiteur::BeginPlay(){
	Super::BeginPlay();

	if (ConstructionZoneList.Num() == 0){
		UE_LOG(LogTemp, Fatal, TEXT("L'INHIBITEUR %s N'AFFECTE AUCUNE ZONE DE CONSTRUCTION"), *this->GetName());
	}
}

void AInhibiteur::ActiveObjectif(){
	ActivateLinkedElements(true);
}

void AInhibiteur::DesactivateObjectif(){
	ActivateLinkedElements(false);
}

void AInhibiteur::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	const AGlitchUEGameMode* Gamemode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (Gamemode->GetLevelState() == ELevelState::Normal && Gamemode->GetPhases() == EPhases::Infiltration && !ActivableComp->IsActivated()){
		ActivableComp->ActivateObject();
	}
}

void AInhibiteur::ActivateLinkedElements(const bool bActivate){
	for (int i = 0; i < ConstructionZoneList.Num(); i++) {
		if (bActivate) {
			ConstructionZoneList[i]->GetActivableComp()->ActivateObject();
		}
		else {
			ConstructionZoneList[i]->GetActivableComp()->DesactivateObject();
		}
	}
}
