// Fill out your copyright notice in the Description page of Project Settings.

#include "Objectives/AbstractObjectif.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

AAbstractObjectif::AAbstractObjectif(){
	PrimaryActorTick.bCanEverTick = false;

	MeshObjectif = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshObjectif"));

	SetRootComponent(MeshObjectif);

	MeshObjectif->SetCollisionResponseToAllChannels(ECR_Block);
	MeshObjectif->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	MeshObjectif->SetCanEverAffectNavigation(false);

	AIPerceptionTarget = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AITarget"));

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

	ActivableComp = CreateDefaultSubobject<UActivableComponent>(TEXT("Activable"));

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
}

void AAbstractObjectif::BeginPlay(){
	Super::BeginPlay();

	ActivableComp->OnActivated.AddDynamic(this, &AAbstractObjectif::ActiveObjectif);
	ActivableComp->OnDesactivated.AddDynamic(this, &AAbstractObjectif::DesactivateObjectif);

	InteractableComp->AddInteractable(MeshObjectif);
	InteractableComp->OnInteract.AddDynamic(this, &AAbstractObjectif::Interact);

	HealthComp->OnReciveDamages.AddDynamic(this, &AAbstractObjectif::TakeDamages);
	HealthComp->OnHealthNull.AddDynamic(this, &AAbstractObjectif::HealthNull);
}

void AAbstractObjectif::ActiveObjectif(){}

void AAbstractObjectif::DesactivateObjectif(){}

void AAbstractObjectif::TakeDamages(){}

void AAbstractObjectif::HealthNull(){}

void AAbstractObjectif::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){}

UActivableComponent* AAbstractObjectif::GetActivableComp()  const{
	return ActivableComp;
}

UHealthComponent* AAbstractObjectif::GetHealthComp() const{
	return HealthComp;
}
