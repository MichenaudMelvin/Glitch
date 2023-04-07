// Fill out your copyright notice in the Description page of Project Settings.

#include "PlacableObject/ConstructionZone.h"
#include "PopcornFXEmitterComponent.h"
#include "PopcornFXFunctions.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"

AConstructionZone::AConstructionZone() {
	PrimaryActorTick.bCanEverTick = false;

	ActivableComp = CreateDefaultSubobject<UActivableComponent>(TEXT("Activable"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MedSkelMesh(TEXT("/Game/Meshs/Turrets/Socles/SK_MED_Socle"));
	check(MedSkelMesh.Succeeded());

	GetSkeletalMeshComponent()->SetSkeletalMesh(MedSkelMesh.Object);

	TechMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TechMesh"));
	TechMesh->SetupAttachment(GetSkeletalMeshComponent());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TechSkelMesh(TEXT("/Game/Meshs/Turrets/Socles/SK_TECH_Socle"));
	check(TechSkelMesh.Succeeded());

	TechMesh->SetSkeletalMesh(TechSkelMesh.Object);

	InitialState = EState::Desactivated;

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> FX(TEXT("/Game/VFX/Particles/FX_Environment/Pk_ConstructionZone"));
	check(FX.Succeeded());

	ConstructionEffect = FX.Object;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> ActivAnim(TEXT("/Game/Meshs/Turrets/Socles/AS_Socle"));
	check(ActivAnim.Succeeded());

	ActivationAnim = ActivAnim.Object;
}

void AConstructionZone::BeginPlay(){
	Super::BeginPlay();

	ActivableComp->OnActivated.AddDynamic(this, &AConstructionZone::ActiveObjectif);
	ActivableComp->OnDesactivated.AddDynamic(this, &AConstructionZone::DesactivateObjectif);

	FVector FXLocation = GetActorLocation();
	FXLocation.Z += 5;
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
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FVector SnappedLocation = UKismetMathLibrary::Vector_SnappedToGrid(GetActorLocation(), 200);
	SnappedLocation.Z = GetActorLocation().Z;

	SetActorLocation(SnappedLocation);
}

void AConstructionZone::ActiveObjectif(){
	ConstructionFXEmitter->StartEmitter();
	GetSkeletalMeshComponent()->PlayAnimation(ActivationAnim, false);
	TechMesh->PlayAnimation(ActivationAnim, false);
}

void AConstructionZone::DesactivateObjectif(){
	ConstructionFXEmitter->StopEmitter();
	//faire les anim en reverse
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