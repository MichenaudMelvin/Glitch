// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPursuitDrone/PowerUpDrone.h"
#include "Player/MainPlayerController.h"

APowerUpDrone::APowerUpDrone(){
	DroneMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DroneMesh"));
	SetRootComponent(DroneMesh);

	DroneMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DroneMesh->SetCollisionResponseToAllChannels(ECR_Block);
}

void APowerUpDrone::BeginPlay(){
	Super::BeginPlay();

	SetData(CurrentData);
	InteractableComp->AddInteractable(DroneMesh);
}

void APowerUpDrone::SetMesh(){
	Super::SetMesh();

	DroneMesh->SetSkeletalMesh(Cast<USkeletalMesh>(CurrentData->MeshList[0]));
}

void APowerUpDrone::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	// super is used for selling objects
	// Super::Interact(MainPlayerController, MainPlayer);

	MainPlayer->SetPlacableActorData(CurrentData);
	MainPlayerController->BindConstructionMode();
}
