// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MainAICharacter.h"
#include "FMODBlueprintStatics.h"
#include "PopcornFXFunctions.h"
#include "AI/UI/SightIndication.h"
#include "AI/Waves/WaveManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AMainAICharacter::AMainAICharacter(){
	PrimaryActorTick.bCanEverTick = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("AIHealth"));

	SightComp = CreateDefaultSubobject<USightComponent>(TEXT("SightComponent"));

	SightComp->SetupAttachment(GetMesh());

	SightComp->SetRelativeRotation(FRotator(0, 180, 0));

	IdleFX = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("IdleFX"));
	IdleFX->SetupAttachment(GetMesh());

	HealthComp->OnHealthNull.AddDynamic(this, &AMainAICharacter::HealthNull);

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AMainAICharacter::BeginPlay(){
	Super::BeginPlay();

	FAttachmentTransformRules SightAttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	SightAttachmentRules.LocationRule = EAttachmentRule::KeepRelative;
	SightAttachmentRules.RotationRule = EAttachmentRule::KeepRelative;
	SightAttachmentRules.ScaleRule = EAttachmentRule::KeepWorld;

	const FAttachmentTransformRules FXAttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);

	SightComp->AttachToComponent(GetMesh(), SightAttachmentRules, "Drone_Head");
	IdleFX->AttachToComponent(GetMesh(), FXAttachmentRules, "Drone_Root");

	AIController = Cast<AMainAIController>(GetController());
	Blackboard = AIController->GetBlackboardComponent();
}

void AMainAICharacter::Destroyed(){
	#if WITH_EDITOR
		if(!IsValid(CurrentData)){
			return;
		}
	#endif

	GetWorld()->GetTimerManager().ClearAllTimersForObject(AIController);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), CurrentData->DeathSound, GetActorTransform(), true);
	UPopcornFXFunctions::SpawnEmitterAtLocation(GetWorld(), CurrentData->DeathFX, "PopcornFX_DefaultScene", GetActorLocation(), GetActorRotation());

	Super::Destroyed();
}

void AMainAICharacter::SetCurrentData(UMainAIData* NewData){
	CurrentData = NewData;

	if(!IsValid(CurrentData)){
		return;
	}

	GetMesh()->SetSkeletalMesh(CurrentData->AIMesh);

	IdleFX->SetEffect(CurrentData->IdleFX);
	SightComp->SetStaticMesh(CurrentData->SightMesh);
	SightComp->SetWorldScale3D(CurrentData->SightDetectionScale);

	GetCharacterMovement()->MaxWalkSpeed = CurrentData->Speed;
}

UBlackboardComponent* AMainAICharacter::GetBlackBoard() const{
	return Blackboard;
}

UPopcornFXEmitterComponent* AMainAICharacter::GetIdleFX() const{
	return IdleFX;
}

void AMainAICharacter::StunAI() {
	Blackboard->SetValueAsBool(FName(TEXT("IsStun")), true);
}

void AMainAICharacter::HealthNull() {

	AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	#if WITH_EDITOR
	// uniquement pour le mode spectateur
	if(!IsValid(Player)){
		return;
	}
	#endif

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

void AMainAICharacter::ReceiveGlitchUpgrade(){
	IGlitchInterface::ReceiveGlitchUpgrade();
	// Ici set les upgrades dans les fonctions qui vont hériter

	GetCharacterMovement()->MaxWalkSpeed = CurrentData->GlitchSpeed;
	SightComp->SetWorldScale3D(CurrentData->GlitchSightScaleDetection);

	HealthComp->SetMaxHealth(CurrentData->GlitchHealth);
	AIController->ToggleGlitchDamages(true);

	FTimerHandle TimerHandle;

	GetWorldTimerManager().SetTimer(TimerHandle, [&]() {
		//reset à l'upgrade actuelle
		ResetGlitchUpgrade();
	}, CurrentData->GlitchDuration, false);
}


void AMainAICharacter::ResetGlitchUpgrade(){
	IGlitchInterface::ResetGlitchUpgrade();

	GetCharacterMovement()->MaxWalkSpeed = CurrentData->Speed;
	SightComp->SetWorldScale3D(CurrentData->SightDetectionScale);

	HealthComp->SetMaxHealth(HealthComp->GetOriginalMaxHealth());
	AIController->ToggleGlitchDamages(false);

}

void AMainAICharacter::ReceiveTrapEffect(const UTrapData* TrapData){
	if(CurrentTrapEffect != ETrapEffect::None){
		return;
	}

	CurrentTrapEffect = TrapData->TrapEffect;

	TrapEffectFX = UPopcornFXFunctions::SpawnEmitterAttached(TrapData->EffectFX, GetMesh(), "PopcornFX_DefaultScene", "Drone_Root", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true, false);

	switch (CurrentTrapEffect) {
	case ETrapEffect::Burned:
		ReceiveBurnEffect(TrapData->TrapDuration, TrapData->EffectTickRate, TrapData->Damages);
		break;
	case ETrapEffect::Frozen:
		ReceiveFreezeEffect(TrapData->TrapDuration);
		break;
	case ETrapEffect::Poisoned:
		ReceivePoisonEffect(TrapData->TrapDuration);
		break;
	case ETrapEffect::SlowedDown:
		ReceiveSlowEffect(TrapData->TrapDuration, TrapData->SlowSpeed);
		break;
	}
}

void AMainAICharacter::ReceiveBurnEffect(const float EffectDuration, const float EffectTickRate, const float EffectDamages){
	GetWorld()->GetTimerManager().SetTimer(EffectTimer, [&]() {
		if(!IsValid(this)){
			GetWorld()->GetTimerManager().ClearTimer(TrapTimer);
			GetWorld()->GetTimerManager().ClearTimer(EffectTimer);
			return;
		}

		HealthComp->TakeDamages(EffectDamages);

	}, EffectTickRate, true);

	GetWorld()->GetTimerManager().SetTimer(TrapTimer, this, &AMainAICharacter::ResetTrapEffect, EffectDuration, false);
}

void AMainAICharacter::ResetBurnEffect(){
	GetWorld()->GetTimerManager().ClearTimer(EffectTimer);
}

void AMainAICharacter::ReceiveFreezeEffect(const float EffectDuration){
	Blackboard->SetValueAsBool("DoingExternalActions", true);

	GetWorld()->GetTimerManager().SetTimer(TrapTimer, this, &AMainAICharacter::ResetTrapEffect, EffectDuration, false);
}

void AMainAICharacter::ResetFreezeEffect(){
	Blackboard->SetValueAsBool("DoingExternalActions", false);
}

void AMainAICharacter::ReceivePoisonEffect(const float EffectDuration){
	Blackboard->SetValueAsBool("ReceivePoisonEffect", true);

	GetWorld()->GetTimerManager().SetTimer(TrapTimer, this, &AMainAICharacter::ResetTrapEffect, EffectDuration, false);
}

void AMainAICharacter::ResetPoisonEffect(){
	Blackboard->SetValueAsBool("ReceivePoisonEffect", false);
}

void AMainAICharacter::ReceiveSlowEffect(const float EffectDuration, const float SlowSpeed){
	GetCharacterMovement()->MaxWalkSpeed = SlowSpeed;

	GetWorld()->GetTimerManager().SetTimer(TrapTimer, this, &AMainAICharacter::ResetTrapEffect, EffectDuration, false);
}

void AMainAICharacter::ResetSlowEffect(){
	GetCharacterMovement()->MaxWalkSpeed = CurrentData->Speed;
}

void AMainAICharacter::ResetTrapEffect(){
	if(!IsValid(this)){
		GetWorld()->GetTimerManager().ClearTimer(TrapTimer);
		return;
	}

	// temporary check
	if(IsValid(TrapEffectFX)){
		TrapEffectFX->DestroyComponent();
	}

	switch (CurrentTrapEffect) {
	case ETrapEffect::Burned:
		ResetBurnEffect();
		break;
	case ETrapEffect::Frozen:
		ResetFreezeEffect();
		break;
	case ETrapEffect::Poisoned:
		ResetPoisonEffect();
		break;
	case ETrapEffect::SlowedDown:
		ResetSlowEffect();
		break;
	}

	CurrentTrapEffect = ETrapEffect::None;
}
