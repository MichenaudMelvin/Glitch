// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FocusAICharacter.h"
#include "AI/FocusAIData.h"

AFocusAICharacter::AFocusAICharacter(){
	HealthWidget = CreateDefaultSubobject<UBillboardWidgetComponent>(TEXT("Health Bar"));
	HealthWidget->SetupAttachment(GetMesh());

	HealthWidget->SetRelativeLocation(FVector(0, 0, 100));
	HealthWidget->SetRelativeRotation(FRotator(0, 180, 0));
	HealthWidget->SetDrawSize(FVector2D(100, 20));

	HealthComp->OnHealthChange.AddDynamic(this, &AFocusAICharacter::UpdateWidgetHealth);

	AttackFX = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("Attack FX"));
	AttackFX->SetupAttachment(GetMesh());
	AttackFX->bPlayOnLoad = false;

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> AttackEffect(TEXT("/Game/VFX/Particles/FX_Enemies/Drones/Pk_AttackingDrone_Attack"));
	check(AttackEffect.Succeeded());

	AttackFX->SetEffect(AttackEffect.Object);

	AttackAudioComp = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Attack Audio"));
	AttackAudioComp->SetupAttachment(GetMesh());

	HitFX = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("Hit FX"));
	HitFX->SetupAttachment(GetMesh());
	HitFX->bPlayOnLoad = false;

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> HitEffect(TEXT("/Game/VFX/Particles/FX_Enemies/Pk_HitEffect"));
	check(HitEffect.Succeeded());

	HitFX->SetEffect(HitEffect.Object);
}

void AFocusAICharacter::BeginPlay(){
	Super::BeginPlay();

	HealthComp->OnReciveDamages.AddDynamic(this, &AFocusAICharacter::TakeDamages);

	EnemyHealthBarWidget = Cast<UEnemyHealthBar>(HealthWidget->GetWidget());
	UpdateWidgetHealth();
}

void AFocusAICharacter::UpdateWidgetHealth(){
	if(IsValid(EnemyHealthBarWidget)){
		EnemyHealthBarWidget->UpdateHealth(HealthComp->GetNormalizedHealth());
	}
}

void AFocusAICharacter::SetCurrentData(UMainAIData* NewData){
	Super::SetCurrentData(NewData);

	if(!IsValid(CurrentData)){
		return;
	}

	const UFocusAIData* FocusData = Cast<UFocusAIData>(CurrentData);

	AttackAudioComp->SetEvent(FocusData->AttackSound);

	UpdateWidgetHealth();
}

void AFocusAICharacter::TakeDamages(){
	HitFX->StartEmitter();
}

UPopcornFXEmitterComponent* AFocusAICharacter::GetAttackFX() const{
	return AttackFX;
}

UFMODAudioComponent* AFocusAICharacter::GetAttackAudioComp() const{
	return AttackAudioComp;
}
