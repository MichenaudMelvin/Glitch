// Fill out your copyright notice in the Description page of Project Settings.


#include "Mark/Mark.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayer.h"

AMark::AMark() {
	PrimaryActorTick.bCanEverTick = false;

	MarkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MarkMesh"));
	SetRootComponent(MarkMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Engine/EditorMeshes/ArcadeEditorSphere"));
	check(Mesh.Object);

	MarkMesh->SetStaticMesh(Mesh.Object);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = 0;
	ProjectileMovement->Friction = 0;
	ProjectileMovement->BounceVelocityStopSimulatingThreshold = 0;
}

void AMark::BeginPlay(){
	Super::BeginPlay();

	StopProjectile();
}

void AMark::StartProjectile() const{
	FVector NewVelocity = FVector::ZeroVector;
	NewVelocity.X = ProjectileMovement->MaxSpeed;
	ProjectileMovement->SetVelocityInLocalSpace(NewVelocity);
}

void AMark::StopProjectile() const{
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ZeroVector);
}

bool AMark::LocationTrace(const float UpTraceValue, FVector& OutImpactPoint){
	FHitResult HitResult;

	FVector TraceEnd = GetActorLocation();
	TraceEnd.Z += UpTraceValue;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	const bool bHit = UKismetSystemLibrary::LineTraceSingle(this, GetActorLocation(), TraceEnd, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true, FLinearColor::Red, FLinearColor::Green, 0);
	OutImpactPoint = HitResult.ImpactPoint;

	return bHit;
}

bool AMark::GetIsMarkPlaced() const{
	return bIsMarkPlaced;
}

void AMark::PlaceMark(){
	bIsMarkPlaced = true;
	StopProjectile();
}

void AMark::ResetMark(){
	bIsMarkPlaced = false;
	StopProjectile();
}

void AMark::Launch(const FRotator StartRotation){
	SetActorRotation(StartRotation);
	StartProjectile();
}
