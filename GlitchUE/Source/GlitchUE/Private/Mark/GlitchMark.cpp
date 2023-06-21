// Fill out your copyright notice in the Description page of Project Settings.


#include "Mark/GlitchMark.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/MainPlayer.h"
#include "Player/MainPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AGlitchMark::AGlitchMark(){
	PrimaryActorTick.bCanEverTick = true;

	MarkMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	MarkFX = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("Wisp"));
	MarkFX->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> FX(TEXT("/Game/VFX/Particles/FX_Environment/Pk_FeuFollet"));
	check(FX.Object);

	MarkFX->SetEffect(FX.Object);
	MarkFX->bPlayOnLoad = false;

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("MarkInteraction"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_SmoothZeroToOneCurve"));
	check(Curve.Succeeded());

	FOnTimelineFloat UpdateEvent;
	FOnTimelineEvent FinishedEvent;

	UpdateEvent.BindDynamic(this, &AGlitchMark::DistanceFromMark);
	FinishedEvent.BindDynamic(this, &AGlitchMark::ResetMark);

	DistanceFromTheMarkTimeline.AddInterpFloat(Curve.Object, UpdateEvent);
	DistanceFromTheMarkTimeline.SetTimelineFinishedFunc(FinishedEvent);
}

void AGlitchMark::BeginPlay(){
	Super::BeginPlay();

	DistanceFromTheMarkTimeline.SetPlayRate(1/GoBackToPlayerDuration);

	InteractableComp->AddInteractable(MarkMesh);
	InteractableComp->OnInteract.AddDynamic(this, &AGlitchMark::Interact);

	FakeMark = Cast<AFakeMark>(UGameplayStatics::GetActorOfClass(this, AFakeMark::StaticClass()));

#if WITH_EDITOR
	if(!IsValid(FakeMark)){
		GEngine->AddOnScreenDebugMessage(-1, 999999999.0f, FColor::Yellow, TEXT("AUCUNE FAKE MARK PLACE DANS LA SCENE"));
		UE_LOG(LogTemp, Warning, TEXT("AUCUNE FAKE MARK PLACE DANS LA SCENE"));
	}
#endif

#if WITH_EDITOR
	// Check pour le mode simulation
	if (!IsValid(UGameplayStatics::GetPlayerCharacter(this, 0))){
		return;
	}
#endif

	Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	Player->SetMark(this);

	AttachToPlayer();

	FWorldDelegates::OnWorldCleanup.AddUFunction(this, "OnCleanWorld");
}

void AGlitchMark::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);

	DistanceFromTheMarkTimeline.TickTimeline(DeltaSeconds);
}

void AGlitchMark::OnCleanWorld(UWorld* World, bool bSessionEnded, bool bCleanupResources)	{
	World->GetTimerManager().ClearTimer(DistanceTimerHandle);
}

void AGlitchMark::AttachToPlayer(){
	if(!bCanBeAttached){
		return;
	}

	AttachToComponent(Player->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
	MarkFX->StartEmitter();
	MarkMesh->SetVisibility(false, false);
	RootComponent->SetRelativeLocation(RelativeLocation);
}

void AGlitchMark::DetachToPlayer(){
	if(!bCanBeAttached){
		return;
	}

	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MarkMesh->SetVisibility(true, false);
	MarkFX->StopEmitter();
}

void AGlitchMark::ReattachToPlayer(){
	bCanBeAttached = true;

	AttachToPlayer();
}

void AGlitchMark::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	ResetMark();
}

FVector AGlitchMark::GetTPLocation(){
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

void AGlitchMark::PlaceMark(){
	if(bIsMarkPlaced){
		return;
	}

	Super::PlaceMark();

	FakeMark->PlaceMark();
	MarkMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	DistanceFromTheMarkTimeline.Stop();
}

void AGlitchMark::ResetMark(){
	Super::ResetMark();

	MarkMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	AttachToPlayer();
	Player->GetMainPlayerController()->BindGlitch();
}

void AGlitchMark::Launch(const FRotator StartRotation){
	Super::Launch(StartRotation);

	FakeMark->SetActorLocation(Player->GetFollowCamera()->GetComponentLocation());

	DetachToPlayer();
	GetWorldTimerManager().SetTimer(DistanceTimerHandle, this, &AGlitchMark::DistanceTimer, 0.1f, true);
}

void AGlitchMark::DistanceTimer(){
	if(GetDistanceTo(Player) > MaxLaunchDistance){
		GetWorldTimerManager().ClearTimer(DistanceTimerHandle);

		LastPosition = GetActorLocation();

		DistanceFromTheMarkTimeline.PlayFromStart();
	}
}

void AGlitchMark::DistanceFromMark(float Value){
	SetActorLocation(UKismetMathLibrary::VLerp(LastPosition, Player->GetActorLocation(), Value));
}

float AGlitchMark::GetMaxLaunchDistance() const{
	return MaxLaunchDistance;
}

UProjectileMovementComponent* AGlitchMark::GetProjectileMovementComp() const{
	return ProjectileMovement;
}

AFakeMark* AGlitchMark::GetFakeMark() const{
	return FakeMark;
}
