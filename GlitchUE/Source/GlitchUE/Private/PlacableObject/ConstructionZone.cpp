// Fill out your copyright notice in the Description page of Project Settings.

#include "PlacableObject/ConstructionZone.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"

AConstructionZone::AConstructionZone() {
	PrimaryActorTick.bCanEverTick = false;

	ActivableComp = CreateDefaultSubobject<UActivableComponent>(TEXT("Activable"));

	UBoxComponent* BoxComp = Cast<UBoxComponent>(GetCollisionComponent());
	BoxComp->SetBoxExtent(FVector::OneVector * 100);

	BoxComp->SetMobility(EComponentMobility::Static);

	InitialState = EState::CPF_Desactivated;
}

void AConstructionZone::BeginPlay(){
	Super::BeginPlay();

	switch (InitialState){
	case EState::CPF_Activated:
		ActivableComp->ActivateObject();
		break;
	case EState::CPF_Desactivated:
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

void AConstructionZone::ToggleActivation(const bool bActivate){
	if (bActivate) {
		ActivableComp->ActivateObject();
	}
	else {
		ActivableComp->DesactivateObject();
	}

	SetActorHiddenInGame(bActivate);
}

void AConstructionZone::ActiveObjectif(){
	ToggleActivation(true);
}

void AConstructionZone::DesactivateObjectif(){
	ToggleActivation(false);
}

void AConstructionZone::OccupiedSlot_Implementation(APlacableActor* NewUnit){
	UnitInZone = NewUnit;
	UnitInZone->SetConstructionZone(this);
}

void AConstructionZone::UnoccupiedSlot_Implementation(){
	UnitInZone = nullptr;
}

UActivableComponent* AConstructionZone::GetActivableComp(){
	return ActivableComp;
}