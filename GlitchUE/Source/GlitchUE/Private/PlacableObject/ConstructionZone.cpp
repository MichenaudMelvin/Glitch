// Fill out your copyright notice in the Description page of Project Settings.

#include "PlacableObject/ConstructionZone.h"
#include "PopcornFXEmitterComponent.h"
#include "PopcornFXFunctions.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"

AConstructionZone::AConstructionZone() {
	PrimaryActorTick.bCanEverTick = false;

	ActivableComp = CreateDefaultSubobject<UActivableComponent>(TEXT("Activable"));

	UBoxComponent* BoxComp = Cast<UBoxComponent>(GetCollisionComponent());
	BoxComp->SetBoxExtent(FVector::OneVector * 100);

	BoxComp->SetMobility(EComponentMobility::Static);

	InitialState = EState::Desactivated;

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> FX(TEXT("/Game/VFX/Particles/FX_Environment/Pk_ConstructionZone"));
	check(FX.Succeeded());

	ConstructionEffect = FX.Object;
}

void AConstructionZone::BeginPlay(){
	Super::BeginPlay();

	ActivableComp->OnActivated.AddDynamic(this, &AConstructionZone::ActiveObjectif);
	ActivableComp->OnDesactivated.AddDynamic(this, &AConstructionZone::DesactivateObjectif);

	FVector FXLocation = GetActorLocation();
	FXLocation.Z = (FXLocation.Z - Cast<UBoxComponent>(GetCollisionComponent())->GetUnscaledBoxExtent().Z) + 5;
	ConstructionFXEmitter = UPopcornFXFunctions::SpawnEmitterAtLocation(GetWorld(), ConstructionEffect, "PopcornFX_DefaultScene", FXLocation, FRotator::ZeroRotator, false, false);

	switch (InitialState){
	case EState::Activated:
		ActivableComp->ActivateObject();
		break;
	case EState::Desactivated:
		ActivableComp->DesactivateObject();
		break; 
	}
}

void AConstructionZone::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	const UBoxComponent* BoxComp = Cast<UBoxComponent>(GetCollisionComponent());

	FVector BoxExtent = UKismetMathLibrary::Vector_SnappedToGrid(BoxComp->GetUnscaledBoxExtent(), 100);
	BoxExtent.Z = 100;
	Cast<UBoxComponent>(GetCollisionComponent())->SetBoxExtent(BoxExtent);
}

void AConstructionZone::ActiveObjectif(){
	ConstructionFXEmitter->StartEmitter();
}

void AConstructionZone::DesactivateObjectif(){
	ConstructionFXEmitter->StopEmitter();
}

void AConstructionZone::OccupiedSlot(APlacableActor* NewUnit){
	UnitInZone = NewUnit;
	UnitInZone->SetConstructionZone(this);
	ConstructionFXEmitter->StopEmitter();
}

void AConstructionZone::UnoccupiedSlot(){
	UnitInZone = nullptr;
	if(ActivableComp->IsActivated()){
		ConstructionFXEmitter->StartEmitter();
	}
}

UActivableComponent* AConstructionZone::GetActivableComp(){
	return ActivableComp;
}