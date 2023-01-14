// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableObject/TurretSMHead.h"
#include "Components/InteractableComponent.h"

ATurretSMHead::ATurretSMHead() {
	TurretHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretHead"));
	TurretHead->SetupAttachment(TurretPillar);
}

void ATurretSMHead::BeginPlay() {
	Super::BeginPlay();

	InteractableComp->AddInteractable(TurretHead);
}

void ATurretSMHead::LookAtTarget(AActor* Target) {
	CurrentPitchRotation = TurretHead->GetComponentRotation().Pitch;

	Super::LookAtTarget(Target);
}

void ATurretSMHead::RotateToTarget(float Alpha) {
	Super::RotateToTarget(Alpha);

	FRotator TargetRotator;

	TargetRotator.Pitch = FMath::Lerp(CurrentPitchRotation, AILookAtRotation.Pitch, Alpha);

	TurretHead->SetWorldRotation(TargetRotator);
}

void ATurretSMHead::SetMesh() {
	Super::SetMesh();

	TurretHead->SetStaticMesh(Cast<UStaticMesh>(CurrentData->MeshList[2]));
}