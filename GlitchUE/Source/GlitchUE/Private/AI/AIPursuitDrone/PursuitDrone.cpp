// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPursuitDrone/PursuitDrone.h"
#include "Components/CapsuleComponent.h"
#include "Player/MainPlayer.h"

APursuitDrone::APursuitDrone(){
	PrimaryActorTick.bCanEverTick = true;

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interaction"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DroneSkeletal(TEXT("/Game/Meshs/Drones/SK_Drones_Pursuit"));
	check(DroneSkeletal.Succeeded());

	GetMesh()->SetSkeletalMesh(DroneSkeletal.Object);
	GetMesh()->SetRelativeRotation(FRotator(0, 180, 0));

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Anim(TEXT("/Game/Meshs/Drones/AS_Drones_Pursuit_Start"));
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
	AIController->OnStopBehavior.AddDynamic(this, &APursuitDrone::TransformIntoPowerUp);
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
}

void APursuitDrone::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);

	SpinTimeline.TickTimeline(DeltaSeconds);
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

void APursuitDrone::OnTouchSomething(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		const AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);

		if(!Player->IsInGlitchZone() && Player->GetHealthComp()->GetCanTakeDamages()){
			Player->GetHealthComp()->TakeDamages(AIController->GetDamages());
			Destroy();
		}
	}
}

void APursuitDrone::TransformIntoPowerUp(){
	GetCapsuleComponent()->OnComponentBeginOverlap.Clear();

	InteractableComp->AddInteractable(GetMesh());
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);
}

void APursuitDrone::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	EnableSpinBehavior();

	MainPlayer->SetCurrentDrone(this);
}

void APursuitDrone::EnableSpinBehavior(){
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	GetMesh()->SetWorldScale3D(SpinScale);
	GetMesh()->SetRelativeLocation(FVector(0, SpinOffset, 0));
	GetMesh()->SetRelativeRotation(FRotator(0, -180, InteriorRotationOffset));
	SpinTimeline.Play();
}

void APursuitDrone::DisableSpinBehavior(){
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);

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

void APursuitDrone::BoostPlacable(){
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void APursuitDrone::Spin(float Value){
	const float CurrentYaw = FMath::Lerp(Value, 0.0f, 360.0f);
	SetActorRotation(FRotator(0, CurrentYaw, 0));
}
