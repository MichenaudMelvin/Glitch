// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MainAICharacter.h"
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

	SightWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("SightWidget"));
	SightWidget->SetupAttachment(GetMesh());

	SightWidget->SetRelativeLocation(FVector(0, 0, 200));
	SightWidget->SetRelativeRotation(FRotator(0, 90, 0));

	SightWidget->SetWorldScale3D(FVector(0.25,0.05,0.05));
	SightWidget->SetDrawSize(FVector2D(1920, 1080));

	SightWidget->SetGenerateOverlapEvents(false);

	IdleFX = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("IdleFX"));
	IdleFX->SetupAttachment(GetMesh());
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

	HealthComp->OnHealthNull.AddDynamic(this, &AMainAICharacter::HealthNull);

	// USightIndication* Widget = Cast<USightIndication>(SightWidget->GetWidget());
	// SightComp->OnSightPlayer.AddDynamic(Widget, &USightIndication::UpdateSightIndication);
	// SightComp->OnLooseSightPlayer.AddDynamic(Widget, &USightIndication::UpdateSightIndication);
}

void AMainAICharacter::Destroyed(){
	#if WITH_EDITOR
		if(!IsValid(CurrentData)){
			return;
		}
	#endif

	GetWorld()->GetTimerManager().ClearAllTimersForObject(AIController);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

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
			if(!IsValid(this)){
				GetWorld()->GetTimerManager().ClearTimer(TrapTimer);
				return;
			}

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
			if(!IsValid(this)){
				GetWorld()->GetTimerManager().ClearTimer(TrapTimer);
				return;
			}

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
			if(!IsValid(this)){
				GetWorld()->GetTimerManager().ClearTimer(TrapTimer);
				return;
			}

			CurrentTrapEffect = ETrapEffect::None;
			GetCharacterMovement()->MaxWalkSpeed = 200;
		}, EffectDuration, false);

		break;
	}
}