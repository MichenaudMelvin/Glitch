// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MainAICharacter.h"
#include "AI/Waves/WaveManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AMainAICharacter::AMainAICharacter(){
	PrimaryActorTick.bCanEverTick = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("AIHealth"));
}

void AMainAICharacter::BeginPlay(){
	Super::BeginPlay();
	
	AIController = Cast<AMainAIController>(GetController());
	Blackboard = AIController->GetBlackboardComponent();
	Blackboard->SetValueAsVector(FName(TEXT("OriginalPosition")), GetActorLocation());

	HealthComp->OnHealthNull.AddDynamic(this, &AMainAICharacter::HealthNull);
}

void AMainAICharacter::InitializeAI(FTransform NewTransform, UBlackboardData* NewBlackBoard){
	SetActorTransform(NewTransform);
	AIController->UseBlackboard(NewBlackBoard, Blackboard);
}

void AMainAICharacter::StunAI() {
	Blackboard->SetValueAsBool(FName(TEXT("IsStun")), true);
}

void AMainAICharacter::HealthNull() {
	Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GiveGolds(10);

	if (IsValid(WaveManager)) {
		WaveManager->RemoveAIFromList(this);
	}

	Destroy();
}

void AMainAICharacter::SetWaveManager(AWaveManager* NewWaveManager) {
	WaveManager = NewWaveManager;
}

AMainAIController* AMainAICharacter::GetMainAIController() const{
	return AIController;
}

UHealthComponent* AMainAICharacter::GetHealthComp() const{
	return HealthComp;
}

void AMainAICharacter::GlitchUpgrade_Implementation(){
	// Ici set les upgrades dans les fonctions qui vont hériter

	FTimerHandle TimerHandle;

	GetWorldTimerManager().SetTimer(TimerHandle, [&]() {
		//reset à l'upgrade actuelle
		ResetGlitchUpgrade();
	}, GlitchUpgradeDuration, false);
}

void AMainAICharacter::ResetGlitchUpgrade_Implementation(){}

void AMainAICharacter::ReceiveTrapEffect(const ETrapEffect NewEffect, const float EffectDuration, const float EffectTickRate, const float EffectDamages){
	if(CurrentTrapEffect != ETrapEffect::None){
		return;
	}

	CurrentTrapEffect = NewEffect;

	// hard codé
	// à voir comment mieux faire
	
	switch (CurrentTrapEffect) {
	case ETrapEffect::Burned: 
		GetWorld()->GetTimerManager().SetTimer(EffectTimer, [&]() {
			HealthComp->TakeDamages(1);
			UE_LOG(LogTemp, Warning, TEXT("Take burn damages"));
		}, EffectTickRate, true);

		GetWorld()->GetTimerManager().SetTimer(TrapTimer, [&]() {
			CurrentTrapEffect = ETrapEffect::None;
			GetWorld()->GetTimerManager().ClearTimer(EffectTimer);
		}, EffectDuration, false);
		
		break;
	case ETrapEffect::Frozen: 
		Blackboard->SetValueAsBool("DoingExternalActions", true);

		GetWorld()->GetTimerManager().SetTimer(TrapTimer, [&]() {
			CurrentTrapEffect = ETrapEffect::None;
			Blackboard->SetValueAsBool("DoingExternalActions", false);
		}, EffectDuration, false);
		
		break;
	case ETrapEffect::Poisoned:
		UE_LOG(LogTemp, Warning, TEXT("poison"));

		break;
	case ETrapEffect::SlowedDown: 
		GetCharacterMovement()->MaxWalkSpeed = 50;

		GetWorld()->GetTimerManager().SetTimer(TrapTimer, [&]() {
			CurrentTrapEffect = ETrapEffect::None;
			GetCharacterMovement()->MaxWalkSpeed = 200;
		}, EffectDuration, false);

		break;
	}
}
