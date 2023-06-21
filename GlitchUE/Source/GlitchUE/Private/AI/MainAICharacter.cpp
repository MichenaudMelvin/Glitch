// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MainAICharacter.h"
#include "FMODBlueprintStatics.h"
#include "PopcornFXFunctions.h"
#include "AI/UI/SightIndication.h"
#include "AI/Waves/WaveManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PlacableObject/TrapData.h"
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

	IdleAudioComp = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Idle Audio"));
	IdleAudioComp->SetupAttachment(GetMesh());

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

	// this avoid a warns
	// also destroying component may improve performances if unused
	IsValid(IdleFX->Effect) ? IdleFX->AttachToComponent(GetMesh(), FXAttachmentRules, "Drone_Root") : IdleFX->DestroyComponent();

	AIController = Cast<AMainAIController>(GetController());
	Blackboard = AIController->GetBlackboardComponent();

	FWorldDelegates::OnWorldCleanup.AddUFunction(this, "OnCleanWorld");
}

void AMainAICharacter::Destroyed(){
	#if WITH_EDITOR
		if(!IsValid(CurrentData)){
			return;
		}
	#endif

	GetWorld()->GetTimerManager().ClearAllTimersForObject(AIController);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	TriggerDeathEffects();

	if(IsValid(TrapEffectFX)){
		TrapEffectFX->StopEmitter();
	}

	Super::Destroyed();
}

void AMainAICharacter::SetCurrentData(UMainAIData* NewData){
	CurrentData = NewData;

	if(!IsValid(CurrentData)){
		return;
	}

	HealthComp->SetMaxHealth(CurrentData->Health);

	GetMesh()->SetSkeletalMesh(CurrentData->AIMesh);

	IdleFX->SetEffect(CurrentData->IdleFX);
	IdleAudioComp->SetEvent(CurrentData->IdleSound);
	IdleAudioComp->Play();

	SightComp->SetStaticMesh(CurrentData->SightMesh);
	SightComp->SetWorldScale3D(CurrentData->SightDetectionScale);
	SightComp->SetOrignialScale(CurrentData->SightDetectionScale);

	GetCharacterMovement()->MaxWalkSpeed = CurrentData->Speed;
}

UBlackboardComponent* AMainAICharacter::GetBlackBoard() const{
	return Blackboard;
}

UPopcornFXEmitterComponent* AMainAICharacter::GetIdleFX() const{
	return IdleFX;
}

void AMainAICharacter::OnCleanWorld(UWorld* World, bool bSessionEnded, bool bCleanupResources){
	World->GetTimerManager().ClearTimer(GlitchTimerHandle);
	World->GetTimerManager().ClearTimer(EffectTimer);
	World->GetTimerManager().ClearTimer(TrapTimer);
}

void AMainAICharacter::TriggerDeathEffects() const{
	UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), CurrentData->DeathSound, GetActorTransform(), true);
	UPopcornFXFunctions::SpawnEmitterAtLocation(GetWorld(), CurrentData->DeathFX, "PopcornFX_DefaultScene", GetActorLocation(), GetActorRotation());
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

	GetWorldTimerManager().SetTimer(GlitchTimerHandle, [&]() {
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
		ReceiveBurnEffect(TrapData->TrapEffectDuration, TrapData->EffectTickRate, TrapData->EffectDamages);
		break;
	case ETrapEffect::Frozen:
		ReceiveFreezeEffect(TrapData->TrapEffectDuration);
		break;
	case ETrapEffect::Poisoned:
		ReceivePoisonEffect(TrapData->TrapEffectDuration);
		break;
	case ETrapEffect::SlowedDown:
		ReceiveSlowEffect(TrapData->TrapEffectDuration, TrapData->SlowSpeed);
		break;
	}
}

void AMainAICharacter::ReceiveBurnEffect(const float EffectDuration, const float EffectTickRate, const float EffectDamages){
	TrapEffectDamages = EffectDamages;

	GetWorld()->GetTimerManager().SetTimer(EffectTimer, [&]() {
		if(!IsValid(this) || !IsValid(HealthComp)){
			GetWorld()->GetTimerManager().ClearTimer(TrapTimer);
			GetWorld()->GetTimerManager().ClearTimer(EffectTimer);
			return;
		}

		HealthComp->TakeDamages(TrapEffectDamages);

	}, EffectTickRate, true);

	GetWorld()->GetTimerManager().SetTimer(TrapTimer, this, &AMainAICharacter::ResetTrapEffect, EffectDuration, false);
}

void AMainAICharacter::ResetBurnEffect(){
	GetWorld()->GetTimerManager().ClearTimer(EffectTimer);
}

void AMainAICharacter::ReceiveFreezeEffect(const float EffectDuration){
	Blackboard->SetValueAsBool("StopAI", true);

	GetWorld()->GetTimerManager().SetTimer(TrapTimer, this, &AMainAICharacter::ResetTrapEffect, EffectDuration, false);
}

void AMainAICharacter::ResetFreezeEffect(){
	Blackboard->SetValueAsBool("StopAI", false);
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
		return;
	}

	if(IsValid(TrapEffectFX)){
		TrapEffectFX->StopEmitter();
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
