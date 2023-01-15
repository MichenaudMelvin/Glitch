// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableObject/PlacableActor.h"
#include "..\..\Public\PlacableObject\PlacableActor.h"
#include "Components/InteractableComponent.h"
#include "Player/MainPlayer.h"
#include "Player/MainPlayerController.h"

APlacableActor::APlacableActor(){
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
}

void APlacableActor::BeginPlay(){
	Super::BeginPlay();

	InteractableComp->OnInteract.AddDynamic(this, &APlacableActor::Interact);
	InteractableComp->AddInteractable(BaseMesh);
}

void APlacableActor::SetMesh() {
	BaseMesh->SetStaticMesh(Cast<UStaticMesh>(CurrentData->MeshList[0]));
}

void APlacableActor::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	if (MainPlayerController->GetGameplayMode() == EGameplayMode::CPF_Destruction) {
		SellObject(MainPlayer);
	}
}

void APlacableActor::SellObject(AMainPlayer* MainPlayer){
	MainPlayer->GiveGolds(CurrentData->Cost);
	Destroy();
}

void APlacableActor::SetData(UPlacableActorData* NewData){
	CurrentData = NewData;
	Name = CurrentData->Name;
	SetMesh();
}

void APlacableActor::Upgrade(){
	SetData(CurrentData->NextUpgrade);
}

void APlacableActor::GlitchUpgrade(){
	// Ici set les upgrades dans les fonctions qui vont hériter

	FTimerHandle TimerHandle;

	GetWorldTimerManager().SetTimer(TimerHandle, [&]() {
		//reset à l'upgrade actuelle
		SetData(CurrentData);
	}, CurrentData->GlitchUpgradeDuration, false);
}