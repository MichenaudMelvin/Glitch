// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPursuitDrone/PursuitDrone.h"

#include "AI/AIPursuitDrone/PursuitDroneData.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FX/Dissolver.h"
#include "Player/MainPlayer.h"

APursuitDrone::APursuitDrone(){
	PrimaryActorTick.bCanEverTick = true;

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interaction"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DroneSkeletal(TEXT("/Game/Meshs/Drones/Pursuit/SK_Drones_Pursuit"));
	check(DroneSkeletal.Succeeded());

	GetMesh()->SetSkeletalMesh(DroneSkeletal.Object);
	GetMesh()->SetRelativeRotation(FRotator(0, 180, 0));
	GetMesh()->SetGenerateOverlapEvents(true);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CrystalSkeletal(TEXT("/Game/Meshs/Drones/Pursuit/SK_Drones_Crystal"));
	check(CrystalSkeletal.Succeeded());

	CrystalMesh = CrystalSkeletal.Object;

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

void APursuitDrone::ForceInDock() const{
	GetMesh()->PlayAnimation(StartAnim, false);
	GetMesh()->SetPosition(0);
	GetMesh()->Stop();
	IdleFX->SetVisibility(false);
}

void APursuitDrone::ForceStartAnim() const{
	GetMesh()->PlayAnimation(StartAnim, false);
	GetMesh()->SetPosition(1);
	GetMesh()->Stop();
	IdleFX->SetVisibility(true);
}

void APursuitDrone::SetCurrentData(UMainAIData* NewData){
	Super::SetCurrentData(NewData);

	const UPursuitDroneData* Data = Cast<UPursuitDroneData>(NewData);

	SpinTimeline.SetPlayRate(1/Data->SpinSpeed);
}

void APursuitDrone::OnTouchSomething(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);

		if(Blackboard->GetValueAsBool("IsDocked")){
			return;
		}

		if(!Player->IsInGlitchZone() && Player->CanUpdateGolds()){
			Player->UpdateGolds(AIController->GetDamages(), EGoldsUpdateMethod::TakeDamages);
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
	GetMesh()->SetSkeletalMesh(CurrentData->AIMesh);
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

	const UPursuitDroneData* Data = Cast<UPursuitDroneData>(CurrentData);

	GetMesh()->SetWorldScale3D(Data->SpinScale);
	GetMesh()->SetRelativeLocation(FVector(0, Data->SpinOffset, 0));
	SpinTimeline.Play();
}

void APursuitDrone::DisableSpinBehavior(){
	DetachDrone();

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCharacterMovement()->GravityScale = 1;

	if(!GetMesh()->IsVisible()){
		GetMesh()->SetVisibility(true, true);
	}

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

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
		UE_LOG(LogTemp, Warning, TEXT("parent actor not valid"));
		return;
	}

	if(GetAttachParentActor()->IsA(AMainPlayer::StaticClass())){
		Cast<AMainPlayer>(GetAttachParentActor())->SetCurrentDrone(nullptr);

		const FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
		DetachFromActor(DetachmentTransformRules);
	}

	else if(GetAttachParentActor()->IsA(APlacableActor::StaticClass())){
		FTimerHandle TimerHandle;

		// micro delay for avoid a crash
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){
			Cast<APlacableActor>(GetAttachParentActor())->RemoveDrone(nullptr);

			const FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
			DetachFromActor(DetachmentTransformRules);
		}, 0.1f, false);
	}
}

void APursuitDrone::Spin(float Value){
	const float CurrentYaw = FMath::Lerp(Value, 0.0f, 360.0f);
	SetActorRotation(FRotator(0, CurrentYaw, 0));
}
