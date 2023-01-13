// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableObject/PlacableActor.h"
#include "..\..\Public\PlacableObject\PlacableActor.h"

APlacableActor::APlacableActor(){
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);
}

void APlacableActor::BeginPlay(){
	Super::BeginPlay();
}

void APlacableActor::SetData_Implementation(UPlacableActorData* NewData){
	CurrentData = NewData;
	Name = CurrentData->Name;
	BaseMesh->SetStaticMesh(CurrentData->MeshList[0]);
}

void APlacableActor::Upgrade(){
	SetData(CurrentData->NextUpgrade);
}

void APlacableActor::GlitchUpgrade_Implementation(){
	// Ici set les upgrades dans les fonctions qui vont hériter

	FTimerHandle TimerHandle;

	GetWorldTimerManager().SetTimer(TimerHandle, [&]() {
		//reset à l'upgrade actuel
		SetData(CurrentData);
	}, CurrentData->GlitchUpgradeDuration, false);
}
