// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPursuitDrone/PursuitDrone.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FX/Dissolver.h"
#include "Player/MainPlayer.h"

APursuitDrone::APursuitDrone(){
	PrimaryActorTick.bCanEverTick = true;

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interaction"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DroneSkeletal(TEXT("/Game/Meshs/Drones/Pursuit/SK_Drones_Pursuit"));
	check(DroneSkeletal.Succeeded());

	DroneMesh = DroneSkeletal.Object;

	GetMesh()->SetSkeletalMesh(DroneMesh);
	GetMesh()->SetRelativeRotation(FRotator(0, 180, 0));
	GetMesh()->SetGenerateOverlapEvents(true);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CrystalSkeletal(TEXT("/Game/Meshs/Drones/Pursuit/SK_Drones_Crystal"));
	check(CrystalSkeletal.Succeeded());

	CrystalMesh = CrystalSkeletal.Object;

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> DeathEffect(TEXT("/Game/VFX/Particles/FX_Enemies/Drones/Pk_ExplodingDrone_Destruction"));
	check(DeathEffect.Succeeded());

	DeathFX = DeathEffect.Object;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Anim(TEXT("/Game/Meshs/Drones/Pursuit/AS_Drones_Pursuit_Start"));
	check(Anim.Succeeded());

	StartAnim = Anim.Object;

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_ZeroToOneCurve"));
	check(Curve.Succeeded());

	ZeroToOneCurve = Curve.Object;
}

float APursuitDrone::GetStartAnimDuration() const{
	return StartAnim->GetMaxCurrentTime();
}

void APursuitDrone::BeginPlay(){
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APursuitDrone::OnTouchSomething);

	AIController->OnStopBehavior.AddDynamic(this, &APursuitDrone::StopPursuitBehavior);
	InteractableComp->OnInteract.AddDynamic(this, &APursuitDrone::Interact);

	FOnTimelineFloat UpdateEvent;

	UpdateEvent.BindDynamic(this, &APursuitDrone::Spin);

	SpinTimeline.AddInterpFloat(ZeroToOneCurve, UpdateEvent);
	SpinTimeline.SetLooping(true);
	SpinTimeline.SetPlayRate(1/SpinSpeed);

	#if	WITH_EDITOR

	if (!IsValid(Pad)){
		UE_LOG(LogTemp, Fatal, TEXT("%s NE POSSEDE AUCUN PAD"), *this->GetName());
	}

	#endif

	PlayStartAnim(true);
	IdleFX->SetVisibility(false);
}

void APursuitDrone::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);

	SpinTimeline.TickTimeline(DeltaSeconds);
}

void APursuitDrone::OnConstruction(const FTransform& Transform){
	Super::OnConstruction(Transform);

	if(IsValid(Pad)){
		Pad->SetCurrentDrone(this);

		FTransform TargetTransform = GetActorTransform();
		FVector TargetVector = TargetTransform.GetLocation();
		TargetVector.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		TargetTransform.SetLocation(TargetVector);

		FRotator TargetRotator = TargetTransform.GetRotation().Rotator();

		TargetRotator.Yaw += 180;

		TargetTransform.SetRotation(TargetRotator.Quaternion());

		Pad->SetActorTransform(TargetTransform);
	}
}

void APursuitDrone::PlayStartAnim(const bool bReverseAnim) const{
	// for some reasons when reversing the anim, it should be looped
	GetMesh()->PlayAnimation(StartAnim, bReverseAnim);
	GetMesh()->SetPlayRate(bReverseAnim ? -1 : 1);

	Pad->PlayAnim(bReverseAnim);

	if(bReverseAnim){
		FTimerHandle TimerHandle;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, GetMesh(), &USkeletalMeshComponent::Stop, StartAnim->GetMaxCurrentTime() - 0.1f, false);
	}
}

void APursuitDrone::SetCurrentPad(APursuitDronePad* NewPad){
	Pad = NewPad;
}

void APursuitDrone::OnTouchSomething(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		const AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);

		if(Blackboard->GetValueAsBool("IsDocked")){
			return;
		}

		if(!Player->IsInGlitchZone() && Player->GetHealthComp()->GetCanTakeDamages()){
			Player->GetHealthComp()->TakeDamages(AIController->GetDamages());
			Destroy();
		}
	}
}

void APursuitDrone::StopPursuitBehavior(){
	GetCapsuleComponent()->OnComponentBeginOverlap.Clear();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APursuitDrone::DroneMeshBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APursuitDrone::DroneMeshEndOverlap);

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetMesh()->GetAnimInstance()->StopAllMontages(0);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);

	IdleFX->DestroyComponent();
}

void APursuitDrone::DroneMeshBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(ADissolver::StaticClass())){
		TransformIntoPowerUp();
	}
}

void APursuitDrone::DroneMeshEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	if(OtherActor->IsA(ADissolver::StaticClass())){
		TransformIntoDrone();
	}
}

void APursuitDrone::TransformIntoPowerUp(){
	GetMesh()->SetSkeletalMesh(CrystalMesh);
	InteractableComp->AddInteractable(GetMesh());
}

void APursuitDrone::TransformIntoDrone(){
	GetMesh()->SetSkeletalMesh(DroneMesh);
	InteractableComp->RemoveInteractable(GetMesh());
	DisableSpinBehavior();
}

void APursuitDrone::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	EnableSpinBehavior();

	MainPlayer->SetCurrentDrone(this);
}

void APursuitDrone::EnableSpinBehavior(){
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	GetCharacterMovement()->GravityScale = 0;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);

	GetMesh()->SetWorldScale3D(SpinScale);
	GetMesh()->SetRelativeLocation(FVector(0, SpinOffset, 0));
	GetMesh()->SetRelativeRotation(FRotator(0, -180, InteriorRotationOffset));
	SpinTimeline.Play();
}

void APursuitDrone::DisableSpinBehavior(){
	DetachDrone();

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCharacterMovement()->GravityScale = 1;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);

	GetMesh()->SetWorldScale3D(FVector::OneVector);
	GetMesh()->SetRelativeLocation(FVector::ZeroVector);
	GetMesh()->SetRelativeRotation(FRotator::ZeroRotator);
	SpinTimeline.Stop();
}

UInteractableComponent* APursuitDrone::GetInteractableComp() const{
	return InteractableComp;
}

void APursuitDrone::AttachDrone(AActor* ActorToAttach, const FName SocketName){
	const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
	AttachToActor(ActorToAttach, AttachmentRules, SocketName);
}

void APursuitDrone::DetachDrone(){
	if(!IsValid(GetAttachParentActor())){
		return;
	}

	if(GetAttachParentActor()->IsA(AMainPlayer::StaticClass())){
		Cast<AMainPlayer>(GetAttachParentActor())->SetCurrentDrone(nullptr);
	}

	else if(GetAttachParentActor()->IsA(APlacableActor::StaticClass())){
		Cast<APlacableActor>(GetAttachParentActor())->RemoveDrone(nullptr);
	}

	const FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
	DetachFromActor(DetachmentTransformRules);
}

void APursuitDrone::Spin(float Value){
	const float CurrentYaw = FMath::Lerp(Value, 0.0f, 360.0f);
	SetActorRotation(FRotator(0, CurrentYaw, 0));
}
