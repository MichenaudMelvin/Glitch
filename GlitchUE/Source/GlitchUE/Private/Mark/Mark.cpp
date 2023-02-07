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
	
	MarkMesh->OnComponentHit.AddDynamic(this, &AMark::OnCompHit);
}

void AMark::BeginPlay() {
	Super::BeginPlay();

	StopProjectile();

	OriginalLocation = GetActorLocation();
	
	// Check pour le mode simulation
	if (IsValid(UGameplayStatics::GetPlayerCharacter(this, 0))){
		Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
		Player->SetMark(this);
	}

	InteractableComp->AddInteractable(MarkMesh);
	InteractableComp->OnInteract.AddDynamic(this, &AMark::Interact);
}

void AMark::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	ResetMark();
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

	//line trace sur le sol
	if (LocationTrace(-PlayerHalffHeight, ImpactPoint)) {
		ImpactPoint.Z += PlayerHalffHeight;
	}
	//line trace sur le plafond
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

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), TraceEnd, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, hitResult, true, FLinearColor::Red, FLinearColor::Green, 0);
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
	GetWorldTimerManager().ClearTimer(DistanceTimer);
}

void AMark::ResetMark(){
	bIsMarkPlaced = false;
	Player->GetMainPlayerController()->BindGlitch();
	GetWorldTimerManager().ClearTimer(DistanceTimer);
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
	if ((GetDistanceToLaunchPoint() >= MaxDistance) && !bIsMarkPlaced){
		ResetMark();
	}
}

float AMark::GetMaxDistance() {
	return MaxDistance;
}