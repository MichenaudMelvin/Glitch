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
	check(Mesh.Object);

	MarkMesh->SetStaticMesh(Mesh.Object);

	MarkFX = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("Wisp"));
	MarkFX->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> FX(TEXT("/Game/VFX/Particles/FX_Environment/Pk_FeuFollet"));
	check(FX.Object);

	MarkFX->SetEffect(FX.Object);
	MarkFX->bPlayOnLoad = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("MarkInteraction"));

	MarkMesh->OnComponentHit.AddDynamic(this, &AMark::OnCompHit);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_SmoothZeroToOneCurve"));
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
	AttachToPlayer();

	InteractableComp->AddInteractable(MarkMesh);
	InteractableComp->OnInteract.AddDynamic(this, &AMark::Interact);
}

void AMark::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);

	DistanceFromTheMarkTimeline.TickTimeline(DeltaSeconds);
}

void AMark::AttachToPlayer(){
	AttachToComponent(Player->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
	MarkFX->StartEmitter();
	MarkMesh->SetVisibility(false, false);
	RootComponent->SetRelativeLocation(RelativeLocation);
}

void AMark::DetachToPlayer(){
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MarkMesh->SetVisibility(true, false);
	MarkFX->StopEmitter();
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
}

void AMark::ResetMark(){
	AttachToPlayer();
	bIsMarkPlaced = false;
	bShouldMarkHitSomething = false;
	Player->GetMainPlayerController()->BindGlitch();

	StopProjectile();
}

void AMark::Launch(const FTransform StartTransform){
	DetachToPlayer();
	SetActorRotation(StartTransform.Rotator());
	StartProjectile();
	GetWorldTimerManager().SetTimer(DistanceTimerHandle, this, &AMark::DistanceTimer, 0.1f, true);
}

void AMark::DistanceTimer(){
	if(GetDistanceTo(Player) > MaxLaunchDistance){
		GetWorldTimerManager().ClearTimer(DistanceTimerHandle);

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
