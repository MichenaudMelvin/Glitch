// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableObject/Trap.h"
#include "PopcornFXEmitterComponent.h"
#include "PopcornFXFunctions.h"
#include "AI/MainAICharacter.h"
#include "PlacableObject/TrapData.h"
#include "Components/InteractableComponent.h"

ATrap::ATrap(){
	TrapMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TrapMesh"));
	SetRootComponent(TrapMesh);

	AudioComp->SetupAttachment(RootComponent);
	CrystalUpgradeFX->SetupAttachment(RootComponent);

	TrapMesh->SetCollisionResponseToAllChannels(ECR_Block);

	ActivableComp = CreateDefaultSubobject<UActivableComponent>(TEXT("Activable"));

	CrystalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Crystal"));
	CrystalMesh->SetupAttachment(TrapMesh);

	TrapMesh->CanCharacterStepUpOn = ECB_No;
	CrystalMesh->CanCharacterStepUpOn = ECB_No;

	TrapMesh->SetCanEverAffectNavigation(false);
	CrystalMesh->SetCanEverAffectNavigation(false);

	static ConstructorHelpers::FObjectFinder<UAnimSequenceBase> Anim(TEXT("/Game/Meshs/Traps/Crystals/AS_Crystal"));
	check(Anim.Succeeded());

	CrystalAnimation = Anim.Object;

	TrapDistance = CreateDefaultSubobject<UBoxComponent>(TEXT("TrapDistance"));
	TrapDistance->SetupAttachment(RootComponent);
	TrapDistance->SetRelativeLocation(FVector(0, 0, 50));

	TrapDistance->SetBoxExtent(FVector::ZeroVector);

	TrapDistance->SetCollisionResponseToAllChannels(ECR_Ignore);
	TrapDistance->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	IdleFX = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("Idle FX"));
	IdleFX->SetupAttachment(TrapMesh);
	IdleFX->bPlayOnLoad = false;
}

void ATrap::BeginPlay(){
	Super::BeginPlay();

	InteractableComp->AddInteractable(TrapMesh);

	TrapDistance->OnComponentBeginOverlap.AddDynamic(this, &ATrap::OnReachVision);
	TrapDistance->OnComponentEndOverlap.AddDynamic(this, &ATrap::OnLeaveVision);

	ActivableComp->OnActivated.AddDynamic(this, &ATrap::OnActivateTrap);
	ActivableComp->OnDesactivated.AddDynamic(this, &ATrap::OnDesactivateTrap);
}

void ATrap::OnCleanWorld(UWorld* World, bool bSessionEnded, bool bCleanupResources){
	Super::OnCleanWorld(World, bSessionEnded, bCleanupResources);

	World->GetTimerManager().ClearTimer(AttackTimerHandle);
	World->GetTimerManager().ClearTimer(ActivationTimerHandle);
	World->GetTimerManager().ClearTimer(ReactivationTimerHandle);
}

void ATrap::OnActivateTrap(){
	Attack();

	GetWorld()->GetTimerManager().SetTimer(ActivationTimerHandle, ActivableComp, &UActivableComponent::DesactivateObject, TrapDuration, false);
}

void ATrap::OnDesactivateTrap(){
	if(IsValid(IdleAnimation)){
		TrapMesh->PlayAnimation(IdleAnimation, true);
	}

	GetWorld()->GetTimerManager().SetTimer(ReactivationTimerHandle, this, &ATrap::CheckCanAttack, ReactivationTimer, false);
}

void ATrap::ReceiveGlitchUpgrade(){
	Super::ReceiveGlitchUpgrade();

	TrapDistance->SetBoxExtent(FVector(AttackRange, AttackRange, 50), true);
}

void ATrap::ResetGlitchUpgrade(){
	Super::ResetGlitchUpgrade();

	TrapDistance->SetBoxExtent(FVector(AttackRange, AttackRange, 50), true);
}

void ATrap::AttachDroneToPlacable(APursuitDrone* NewDrone){
	Super::AttachDroneToPlacable(NewDrone);

	TrapDistance->SetBoxExtent(FVector(AttackRange, AttackRange, 50), true);
}

void ATrap::RemoveDrone(AMainPlayer* MainPlayer){
	Super::RemoveDrone(MainPlayer);

	TrapDistance->SetBoxExtent(FVector(AttackRange, AttackRange, 50), true);
}

void ATrap::SetMesh(){
	Super::SetMesh();

	TrapMesh->SetSkeletalMesh(Cast<USkeletalMesh>(CurrentData->MeshList[0]), true);
	TrapMesh->PlayAnimation(IdleAnimation, true);

	CrystalMesh->SetSkeletalMesh(Cast<USkeletalMesh>(CurrentData->MeshList[1]));
	CrystalMesh->PlayAnimation(CrystalAnimation, true);
	CrystalMesh->SetVectorParameterValueOnMaterials("CrystalColor", FVector(CurrentData->CrystalColor));

	TrapDistance->SetBoxExtent(FVector(AttackRange, AttackRange, 50), true);
}

void ATrap::SetData(UPlacableActorData* NewData){
	Super::SetData(NewData);

	const UTrapData* Data = Cast<UTrapData>(NewData);

	TrapDuration = Data->TrapDuration;
	AttackRate = Data->AttackRate;
	TrapEffect = Data->TrapEffect;
	TrapEffectDuration = Data->TrapEffectDuration;
	ReactivationTimer = Data->ReactivationTimer;

	IdleFX->SetEffect(Data->IdleFX, true);
}

void ATrap::Appear(const bool ReverseEffect, const FOnTimelineEvent AppearFinishEvent){
	TrapMesh->SetSkeletalMesh(Cast<USkeletalMesh>(CurrentData->MeshList[0]), true);

	for(int i = 0; i < TrapMesh->GetNumMaterials(); i++){
		TrapMesh->SetMaterial(i, CurrentData->AppearanceMaterial);
	}

	if(ReverseEffect){
		CrystalMesh->SetVisibility(false);
	}

	Super::Appear(ReverseEffect, AppearFinishEvent);
}

void ATrap::FadeIn(float Alpha){
	Super::FadeIn(Alpha);
	TrapMesh->SetScalarParameterValueOnMaterials("PercentageApparition", Alpha);
}

void ATrap::CheckCanAttack(){
	if(AIList.Num() >= 1) {
		ActivableComp->ActivateObject();
	}
}

void ATrap::Attack_Implementation(){
	if(bIsAppearing){
		return;
	}

	Super::Attack_Implementation();

	TArray<AMainAICharacter*> AIArray = AIList.Array();
	const UTrapData* Data = Cast<UTrapData>(CurrentData);

	if(IsValid(AttackAnimation)){
		TrapMesh->PlayAnimation(AttackAnimation, false);
	}

	AudioComp->Play();

	for(int i = 0; i < AIArray.Num(); i++){
		AIArray[i]->ReceiveTrapEffect(Data);
	}

	AttackFX->StartEmitter();

	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, [&]() {
		if(ActivableComp->IsActivated()){
			Attack_Implementation();
		}
	}, AttackRate, false);
}

void ATrap::OnReachVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	Super::OnReachVision(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (AIList.Num() >= 1 && !ActivableComp->IsActivated() && !GetWorld()->GetTimerManager().IsTimerActive(ReactivationTimerHandle)) {
		ActivableComp->ActivateObject();
	}
}