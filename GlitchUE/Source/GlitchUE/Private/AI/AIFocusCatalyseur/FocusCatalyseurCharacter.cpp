// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIFocusCatalyseur/FocusCatalyseurCharacter.h"

AFocusCatalyseurCharacter::AFocusCatalyseurCharacter(){
	ScopeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Scope"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshScope(TEXT("/Game/Meshs/Drones/Enemies/SM_Scope"));
	check(MeshScope.Succeeded());

	ScopeMesh->SetStaticMesh(MeshScope.Object);

	ScopeMesh->SetupAttachment(GetMesh());
	ScopeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ScopeMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AFocusCatalyseurCharacter::BeginPlay(){
	Super::BeginPlay();

	const FAttachmentTransformRules SightAttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);

	ScopeMesh->AttachToComponent(GetMesh(), SightAttachmentRules, "Drone_Head");
}
