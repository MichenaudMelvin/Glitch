// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Catalyseur.h"
#include "AI/Waves/Spawner.h"
#include "Kismet/GameplayStatics.h"

ACatalyseur::ACatalyseur() {
	TECHMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TECHMesh"));
	TECHMesh->SetCanEverAffectNavigation(false);
}

void ACatalyseur::BeginPlay() {
	Super::BeginPlay();

	TArray<AActor*> NexusTemp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANexus::StaticClass(), NexusTemp);

	Nexus = Cast<ANexus>(NexusTemp[0]);

	if (StateAtWave.EnableAtWave == 0) {
		UE_LOG(LogTemp, Fatal, TEXT("LE CATALYSEUR %s NE COMMENCE A AUCUNE VAGUE"), *this->GetName());
	}

	if (StateAtWave.DisableAtWave == 0) {
		UE_LOG(LogTemp, Fatal, TEXT("LE CATALYSEUR %s NE TERMINE A AUCUNE VAGUE"), *this->GetName());
	}
}

void ACatalyseur::ActiveObjectif(){
	if (Nexus->GetActivableComp()->GetState() == EState::CPF_Activated) {
		for (int i = 0; i < ConstructionZoneList.Num(); i++) {
			ConstructionZoneList[i]->GetActivableComp()->ActivateObject();
		}
	}
}

void ACatalyseur::DesactivateObjectif() {
	for (int i = 0; i < ConstructionZoneList.Num(); i++) {
		ConstructionZoneList[i]->GetActivableComp()->DesactivateObject();
	}
}

void ACatalyseur::HealthNull(){
	ActivableComp->DesactivateObject();
}

FStateAtWave ACatalyseur::GetStateAtWave(){
	return StateAtWave;
}