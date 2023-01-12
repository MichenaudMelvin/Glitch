// Fill out your copyright notice in the Description page of Project Settings.

#include "PlacableObject/ConstructionZone.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"

AConstructionZone::AConstructionZone() {
	PrimaryActorTick.bCanEverTick = false;

	ActivableComp = CreateDefaultSubobject<UActivableComponent>(TEXT("Activable"));

	UBoxComponent* BoxComp = Cast<UBoxComponent>(GetCollisionComponent());
	BoxComp->SetBoxExtent(FVector::OneVector * 200);

	BoxComp->SetMobility(EComponentMobility::Static);

	InitialState = EState::CPF_Desactivated;
}

void AConstructionZone::BeginPlay(){
	Super::BeginPlay();

	switch (ActivableComp->GetState()){
	case EState::CPF_Activated:
		ActivableComp->ActivateObject();
		break;
	case EState::CPF_Desactivated:
		ActivableComp->DesactivateObject();
		break;
	}
}

void AConstructionZone::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	UBoxComponent* BoxComp = Cast<UBoxComponent>(GetCollisionComponent());

	FVector BoxExtent = UKismetMathLibrary::Vector_SnappedToGrid(BoxComp->GetUnscaledBoxExtent(), 200);
	Cast<UBoxComponent>(GetCollisionComponent())->SetBoxExtent(BoxExtent);
}

UActivableComponent* AConstructionZone::GetActivableComp(){
	return ActivableComp;
}