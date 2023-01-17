// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableObject/TurretSKHead.h"
#include "Components/InteractableComponent.h"

ATurretSKHead::ATurretSKHead(){
	TurretHead = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TurretHead"));
	TurretHead->SetupAttachment(TurretPillar);
	TurretHead->SetRelativeLocation(FVector(0, 0, 100));
}

void ATurretSKHead::BeginPlay(){
	Super::BeginPlay();

	InteractableComp->AddInteractable(TurretHead);
}

void ATurretSKHead::LookAtTarget(AActor* Target){
	CurrentPitchRotation = TurretHead->GetComponentRotation().Pitch;
	
	Super::LookAtTarget(Target);
}

void ATurretSKHead::RotateToTarget(float Alpha){
	Super::RotateToTarget(Alpha);

	FRotator TargetRotator = FRotator::ZeroRotator;

	TargetRotator.Pitch = FMath::Lerp(CurrentPitchRotation, AILookAtRotation.Pitch, Alpha);

	TurretHead->SetRelativeRotation(TargetRotator);
}

void ATurretSKHead::SetMesh(){
	Super::SetMesh();
	
	TurretHead->SetSkeletalMesh((Cast<USkeletalMesh>(CurrentData->MeshList[2])), true);
}
