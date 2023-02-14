// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableObject/Trap.h"
#include "PopcornFXEmitterComponent.h"
#include "PopcornFXFunctions.h"
#include "AI/MainAICharacter.h"
#include "PlacableObject/TrapData.h"
#include "Components/InteractableComponent.h"

ATrap::ATrap(){
	ActivableComp = CreateDefaultSubobject<UActivableComponent>(TEXT("Activable"));
}

void ATrap::BeginPlay(){
	Super::BeginPlay();

	TrapDistance = Cast<UBoxComponent>(AddComponentByClass(UBoxComponent::StaticClass(), false, FTransform(), false));
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

void ATrap::OnDesactivateTrap(){}

void ATrap::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	if (!ActivableComp->IsActivated()) {
		ActivableComp->ActivateObject();
	}
}

void ATrap::GlitchUpgrade(){
	TrapAttackRate = Cast<UTrapData>(CurrentData)->UpgradedGlitchAttackRate;

	Super::GlitchUpgrade();
}

void ATrap::SetData(UPlacableActorData* NewData){
	Super::SetData(NewData);

	const UTrapData* Data = Cast<UTrapData>(NewData);

	Damages = Data->TrapDamages;
	TrapDuration = Data->TrapDuration;
	TrapAttackRate = Data->TrapAttackRate;
	TrapEffect = Data->TrapEffect;
	TrapEffectDuration = Data->TrapEffectDuration;

	if(IdleFX == nullptr){
		IdleFX = UPopcornFXFunctions::SpawnEmitterAtLocation(GetWorld(), Data->IdleFX, "PopcornFX_DefaultScene", GetActorLocation(), FRotator::ZeroRotator, true, false);
	}
	
	FTimerHandle TimerHandle;
	// Micro delay pour éviter les problèmes de navigation
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		TrapDistance->SetBoxExtent(FVector(AttackRange, AttackRange, 100), true);
	}, 0.2f, false);
}

void ATrap::Attack_Implementation(){
	Super::Attack_Implementation();

	GEngine->AddOnScreenDebugMessage(-1, TrapAttackRate, FColor::Yellow, TEXT("TrapAttack"));	

	TArray<AMainAICharacter*>AIArray = AIList.Array();
	for(int i = 0; i < AIArray.Num(); i++){
		AIArray[i]->ReceiveTrapEffect(TrapEffect, TrapEffectDuration);
	}

	AttackFX->StartEmitter();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		if(ActivableComp->IsActivated()){
			Attack_Implementation();
		}
	}, TrapAttackRate, false);
}

void ATrap::OnReachVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	Super::OnReachVision(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ATrap::OnLeaveVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	Super::OnLeaveVision(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
}