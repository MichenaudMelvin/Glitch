// Fill out your copyright notice in the Description page of Project Settings.

#include "Objectives/AbstractObjectif.h"

AAbstractObjectif::AAbstractObjectif(){
	PrimaryActorTick.bCanEverTick = true;

	MeshObjectif = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshObjectif"));

	MeshObjectif->SetupAttachment(RootComponent);

	//AIPerceptionTarget = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AITarget"));
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

	ActivableComp = CreateDefaultSubobject<UActivableComponent>(TEXT("Activable"));

}

void AAbstractObjectif::BeginPlay(){
	Super::BeginPlay();

	ActivableComp->OnActivated.AddDynamic(this, &AAbstractObjectif::ActiveObjectif);
	ActivableComp->OnDesactivated.AddDynamic(this, &AAbstractObjectif::DesactivateObjectif);
}

void AAbstractObjectif::ActiveObjectif(){}

void AAbstractObjectif::DesactivateObjectif(){}

void AAbstractObjectif::OnHealthNull(){}

UActivableComponent* AAbstractObjectif::GetActivableComp() {
	return ActivableComp;
}