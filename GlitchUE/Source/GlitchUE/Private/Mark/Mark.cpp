// Fill out your copyright notice in the Description page of Project Settings.


#include "Mark/Mark.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Player/MainPlayerController.h"

AMark::AMark() {
	PrimaryActorTick.bCanEverTick = false;
	MarkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MarkMesh"));
	SetRootComponent(MarkMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Engine/EditorMeshes/ArcadeEditorSphere"));
	MarkMesh->SetStaticMesh(Mesh.Object);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("MarkInteraction"));
}

void AMark::BeginPlay(){
	Super::BeginPlay();

	StopProjectile();

	OriginalLocation = GetActorLocation();

#if WITH_EDITOR
	// Check pour le mode simulation
	if (!IsValid(UGameplayStatics::GetPlayerCharacter(this, 0))){
		return;
	}
#endif

	Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	Player->SetMark(this);

	InteractableComp->AddInteractable(MarkMesh);
	InteractableComp->OnInteract.AddDynamic(this, &AMark::Interact);
}

void AMark::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	ResetMark();
}

void AMark::StartProjectile() const{
	FVector NewVelocity = FVector::ZeroVector;
	NewVelocity.X = ProjectileMovement->MaxSpeed;
	ProjectileMovement->SetVelocityInLocalSpace(NewVelocity);
}

void AMark::StopProjectile() const{
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ZeroVector);
}

FVector AMark::GetTPLocation(){
	const float PlayerHalfHeight = Player->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	FVector ImpactPoint;

	//line trace sur le sol
	if (LocationTrace(-PlayerHalfHeight, ImpactPoint)) {
		ImpactPoint.Z += PlayerHalfHeight;
	}
	//line trace sur le plafond
	else if (LocationTrace(PlayerHalfHeight, ImpactPoint)) {
		ImpactPoint.Z -= PlayerHalfHeight;
	}
	else {
		ImpactPoint = GetActorLocation();
	}

	return ImpactPoint;
}

void AMark::SetTargetLocation(const FVector NewTargetLocation){
	TargetLocation = NewTargetLocation;
}

void AMark::SetHitSomething(const bool bValue){
	bShouldMarkHitSomething = bValue;
}

bool AMark::LocationTrace(const float UpTraceValue, FVector& OutImpactPoint){
	FHitResult HitResult;

	FVector TraceEnd = GetActorLocation();
	TraceEnd.Z += UpTraceValue;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	const bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), TraceEnd, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true, FLinearColor::Red, FLinearColor::Green, 0);
	OutImpactPoint = HitResult.ImpactPoint;

	return bHit;
}

bool AMark::GetIsMarkPlaced() const{
	return bIsMarkPlaced;
}

void AMark::PlaceMark(){
	StopProjectile();
	bIsMarkPlaced = true;
	GetWorldTimerManager().ClearTimer(LaunchTimerHandle);
}

void AMark::ResetMark(){
	bIsMarkPlaced = false;
	bShouldMarkHitSomething = false;
	Player->GetMainPlayerController()->BindGlitch();
	GetWorldTimerManager().ClearTimer(LaunchTimerHandle);

	SetActorLocation(OriginalLocation);
	StopProjectile();
}

void AMark::Launch(const FTransform StartTransform){
	SetActorTransform(StartTransform);
	LaunchLocation = StartTransform.GetLocation();
	StartProjectile();
	GetWorldTimerManager().SetTimer(LaunchTimerHandle, this, &AMark::LaunchTimer, 0.001f, true);
}

float AMark::GetDistanceToLaunchPoint() const{
	return FVector::Dist(LaunchLocation, GetActorLocation());
}

void AMark::LaunchTimer(){
	if(GetActorLocation().Equals(TargetLocation, EqualTolerance) && bShouldMarkHitSomething){
		SetActorLocation(TargetLocation);
		PlaceMark();
	}

	if ((GetDistanceToLaunchPoint() >= MaxDistance) && !bIsMarkPlaced){
		ResetMark();
	}
}

float AMark::GetMaxDistance() const{
	return MaxDistance;
}