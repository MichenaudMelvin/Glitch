// Fill out your copyright notice in the Description page of Project Settings.


#include "Mark/Mark.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

AMark::AMark() {
	PrimaryActorTick.bCanEverTick = false;
	MarkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MarkMesh"));
	SetRootComponent(MarkMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Engine/EditorMeshes/ArcadeEditorSphere"));
	MarkMesh->SetStaticMesh(Mesh.Object);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	
	MarkMesh->OnComponentHit.AddDynamic(this, &AMark::OnCompHit);
}

void AMark::BeginPlay() {
	Super::BeginPlay();

	StopProjectile();

	OriginalLocation = GetActorLocation();
	
	if (IsValid(UGameplayStatics::GetPlayerCharacter(this, 0))){
		Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
		Player->SetMark(this);
	}
}

void AMark::StartProjectile() {
	FVector newVelocity = FVector::ZeroVector;
	newVelocity.X = ProjectileMovement->MaxSpeed;
	ProjectileMovement->SetVelocityInLocalSpace(newVelocity);
}

void AMark::StopProjectile() {
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ZeroVector);
}

FVector AMark::GetTPLocation() {
	float PlayerHalffHeight = Player->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	FVector ImpactPoint;

	if (LocationTrace(-PlayerHalffHeight, ImpactPoint)) {
		ImpactPoint.Z += PlayerHalffHeight;
	}
	else if (LocationTrace(PlayerHalffHeight, ImpactPoint)) {
		ImpactPoint.Z -= PlayerHalffHeight;
	}
	else {
		ImpactPoint = GetActorLocation();
	}

	return ImpactPoint;
}

bool AMark::LocationTrace(float UpTraceValue, FVector& outImpactPoint) {
	FHitResult hitResult;

	FVector TraceEnd = GetActorLocation();
	TraceEnd.Z += UpTraceValue;

	FCollisionQueryParams QueryParams;
	FCollisionResponseParams Responseparam;

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), TraceEnd, ECC_Visibility, QueryParams, Responseparam);

	outImpactPoint = hitResult.ImpactPoint;

	return bHit;
}

void AMark::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	PlaceMark();
}

bool AMark::GetIsMarkPlaced(){
	return bIsMarkPlaced;
}

void AMark::PlaceMark(){
	StopProjectile();
	bIsMarkPlaced = true;
	DistanceTimer.Invalidate();
}

void AMark::ResetMark(){
	bIsMarkPlaced = false;
	Player->GetMainPlayerController()->BindGlitch();
	DistanceTimer.Invalidate();
	// invalidate beam timer
	SetActorLocation(OriginalLocation);
	StopProjectile();
	// reset beam end
}

void AMark::Launch(FTransform StartTransform){
	SetActorTransform(StartTransform);
	LaunchLocation = StartTransform.GetLocation();
	StartProjectile();
	GetWorldTimerManager().SetTimer(DistanceTimer, this, &AMark::CheckDistance, 0.1f, true, 0.0f);
	//ajouter beam timer
}

float AMark::GetDistanceToLaunchPoint() {
	return FVector::Dist(LaunchLocation, GetActorLocation());
}

void AMark::CheckDistance() {
	if ((GetDistanceToLaunchPoint() >= MaxDistance) && bIsMarkPlaced){
		ResetMark();
	}
}

float AMark::GetMaxDistance() {
	return MaxDistance;
}