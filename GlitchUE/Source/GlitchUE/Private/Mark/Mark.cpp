// Fill out your copyright notice in the Description page of Project Settings.


#include "Mark/Mark.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/MainPlayerController.h"

AMark::AMark() {
	PrimaryActorTick.bCanEverTick = true;

	MarkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MarkMesh"));
	SetRootComponent(MarkMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Engine/EditorMeshes/ArcadeEditorSphere"));
	MarkMesh->SetStaticMesh(Mesh.Object);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("MarkInteraction"));

	MarkMesh->OnComponentHit.AddDynamic(this, &AMark::OnCompHit);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_ZeroToOneCurve"));
	check(Curve.Succeeded());

	FOnTimelineFloat UpdateEvent;
	FOnTimelineEvent FinishedEvent;

	UpdateEvent.BindDynamic(this, &AMark::DistanceFromMark);
	FinishedEvent.BindDynamic(this, &AMark::ResetMark);

	DistanceFromTheMarkTimeline.AddInterpFloat(Curve.Object, UpdateEvent);
	DistanceFromTheMarkTimeline.SetTimelineFinishedFunc(FinishedEvent);
}

void AMark::BeginPlay(){
	Super::BeginPlay();

	StopProjectile();

	OriginalLocation = GetActorLocation();
	DistanceFromTheMarkTimeline.SetPlayRate(1/GoBackToPlayerDuration);

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

void AMark::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);

	DistanceFromTheMarkTimeline.TickTimeline(DeltaSeconds);
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

void AMark::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit){
	PlaceMark();
}

bool AMark::GetIsMarkPlaced() const{
	return bIsMarkPlaced;
}

void AMark::PlaceMark(){
	StopProjectile();
	bIsMarkPlaced = true;
	GetWorldTimerManager().ClearTimer(LaunchTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(DistanceFromTheMarkTimerHandle, this, &AMark::CheckDistanceFromMark, 0.1f, true);
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
	if ((GetDistanceToLaunchPoint() >= MaxLaunchDistance) && !bIsMarkPlaced){
		ResetMark();
	}
}

void AMark::CheckDistanceFromMark(){
	if(GetDistanceTo(Player) >= MaxDistanceFromTheMark){
		GetWorldTimerManager().ClearTimer(DistanceFromTheMarkTimerHandle);

		Player->GetMainPlayerController()->OnUseGlitchPressed.Clear();

		LastPosition = GetActorLocation();

		DistanceFromTheMarkTimeline.PlayFromStart();
	}
}

void AMark::DistanceFromMark(float Value){
	SetActorLocation(UKismetMathLibrary::VLerp(LastPosition, Player->GetActorLocation(), Value));
}

float AMark::GetMaxLaunchDistance() const{
	return MaxLaunchDistance;
}
