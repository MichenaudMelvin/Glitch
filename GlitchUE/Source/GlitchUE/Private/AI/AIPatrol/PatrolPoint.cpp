// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPatrol/PatrolPoint.h"

APatrolPoint::APatrolPoint(){
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMeshComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Engine/EditorMeshes/ArcadeEditorSphere"));
	check(Mesh.Succeeded());

	StaticMeshComponent->SetStaticMesh(Mesh.Object);
	StaticMeshComponent->SetWorldScale3D(FVector(0.2, 0.2, 0.2));
	StaticMeshComponent->SetMobility(EComponentMobility::Static);
	
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetGenerateOverlapEvents(false);
	StaticMeshComponent->SetCanEverAffectNavigation(false);
	StaticMeshComponent->bHiddenInGame = true;
}
