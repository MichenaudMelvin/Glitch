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

	TrapMesh->CanCharacterStepUpOn = ECB_No;

	TrapMesh->SetCollisionResponseToAllChannels(ECR_Block);

	ActivableComp = CreateDefaultSubobject<UActivableComponent>(TEXT("Activable"));

	CrystalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Crystal"));
	CrystalMesh->SetupAttachment(TrapMesh);

	CrystalMesh->CanCharacterStepUpOn = ECB_No;

	static ConstructorHelpers::FObjectFinder<UAnimSequenceBase> Anim(TEXT("/Game/Meshs/Traps/Crystals/AS_Crystal"));
	check(Anim.Succeeded());

	CrystalAnimation = Anim.Object;

	TrapDistance = CreateDefaultSubobject<UBoxComponent>(TEXT("TrapDistance"));
	TrapDistance->SetupAttachment(RootComponent);
	TrapDistance->SetRelativeLocation(FVector(0, 0, 50));

	TrapDistance->SetBoxExtent(FVector::ZeroVector);
}

void ATrap::BeginPlay(){
	Super::BeginPlay();

	InteractableComp->AddInteractable(TrapMesh);

	TrapDistance->OnComponentBeginOverlap.AddDynamic(this, &ATrap::OnReachVision);
	TrapDistance->OnComponentEndOverlap.AddDynamic(this, &ATrap::OnLeaveVision);

	ActivableComp->OnActivated.AddDynamic(this, &ATrap::OnActivateTrap);
	ActivableComp->OnDesactivated.AddDynamic(this, &ATrap::OnDesactivateTrap);
}

void ATrap::OnActivateTrap(){
	Attack();
	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		ActivableComp->DesactivateObject();
	}, TrapDuration, false);
}

void ATrap::OnDesactivateTrap(){
	if(IsValid(IdleAnimation)){
		TrapMesh->PlayAnimation(IdleAnimation, true);
	}
}

void ATrap::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	Super::Interact(MainPlayerController, MainPlayer);

	if (!ActivableComp->IsActivated()){
		ActivableComp->ActivateObject();
	}
}

void ATrap::ReceiveGlitchUpgrade(){
	AttackRate = CurrentData->GlitchAttackRate;
	Damages = CurrentData->GlitchDamages;
	AttackRange = CurrentData->GlitchAttackRange;

	TrapDistance->SetBoxExtent(FVector(AttackRange, AttackRange, 50), true);

	Super::ReceiveGlitchUpgrade();
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

	if(IdleFX == nullptr){
		IdleFX = UPopcornFXFunctions::SpawnEmitterAtLocation(GetWorld(), Data->IdleFX, "PopcornFX_DefaultScene", GetActorLocation(), FRotator::ZeroRotator, true, false);
	}
}

void ATrap::Appear(const bool ReverseEffect){
	TrapMesh->SetSkeletalMesh(Cast<USkeletalMesh>(CurrentData->MeshList[0]), true);

	for(int i = 0; i < TrapMesh->GetNumMaterials(); i++){
		TrapMesh->SetMaterial(i, CurrentData->AppearanceMaterial);
	}

	if(ReverseEffect){
		CrystalMesh->SetVisibility(false);
	}

	Super::Appear(ReverseEffect);
}

void ATrap::FadeIn(float Alpha){
	Super::FadeIn(Alpha);
	TrapMesh->SetScalarParameterValueOnMaterials("PercentageApparition", Alpha);
}

void ATrap::Attack_Implementation(){
	Super::Attack_Implementation();

	GEngine->AddOnScreenDebugMessage(-1, AttackRate, FColor::Yellow, TEXT("TrapAttack"));

	TArray<AMainAICharacter*>AIArray = AIList.Array();
	const UTrapData* Data = Cast<UTrapData>(CurrentData);

	if(IsValid(AttackAnimation)){
		TrapMesh->PlayAnimation(AttackAnimation, false);
	}

	for(int i = 0; i < AIArray.Num(); i++){
		AIArray[i]->ReceiveTrapEffect(TrapEffect, TrapEffectDuration, Data->EffectTickRate, Data->EffectDamages);
	}

	AttackFX->StartEmitter();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		if(ActivableComp->IsActivated()){
			Attack_Implementation();
		}
	}, AttackRate, false);
}

void ATrap::OnReachVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	Super::OnReachVision(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ATrap::OnLeaveVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	Super::OnLeaveVision(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
}