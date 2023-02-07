// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MainAICharacter.h"
#include "AI/Waves/WaveManager.h"
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

	AIControllerClass;

	HealthComp->OnHealthNull.AddDynamic(this, &AMainAICharacter::HealthNull);
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

AMainAIController* AMainAICharacter::GetMainAIController(){
	return AIController;
}

UHealthComponent* AMainAICharacter::GetHealthComp(){
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
