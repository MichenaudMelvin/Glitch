// Fill out your copyright notice in the Description page of Project Settings.


#include "FX/Dissolver.h"
#include "FMODEvent.h"
#include "Player/MainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"

ADissolver::ADissolver(){
	PrimaryActorTick.bCanEverTick = true;

	DissolveCollider = CreateDefaultSubobject<USphereComponent>(TEXT("DissolveCollider"));
	SetRootComponent(DissolveCollider);

	DissolveCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	DissolveCollider->CanCharacterStepUpOn = ECB_No;

	FMODAudio = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Border SFX Component"));
	FMODAudio->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UFMODEvent> BorderSFX(TEXT("/Game/FMOD/Events/SFX/SFX_border"));
	check(BorderSFX.Succeeded());

	FMODAudio->Event = BorderSFX.Object;

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_ZeroToOneCurve"));
	check(Curve.Succeeded());

	ZeroToOneCurve = Curve.Object;
}

void ADissolver::BeginPlay(){
	Super::BeginPlay();

	if(IsValid(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))){
		MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

	if(UGameplayStatics::GetGameMode(GetWorld())->IsA(AGlitchUEGameMode::StaticClass())){
		GameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		GameMode->OnSwitchPhases.AddDynamic(this, &ADissolver::OnSwitchPhase);
	}

	FOnTimelineFloat UpdateEvent;

	UpdateEvent.BindDynamic(this, &ADissolver::LerpRadius);

	DissolveTimeline.AddInterpFloat(ZeroToOneCurve, UpdateEvent);

	DissolveTimeline.SetPlayRate(1/FXDuration);

	Radius = 0;
	UpdateShaderFX();
}

void ADissolver::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	DissolveTimeline.TickTimeline(DeltaTime);

	if(bAllowBorderSound){
		BorderSound();
	}
}

void ADissolver::UpdateRadius(){
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPCDissolver, "RadiusDissolve", Radius);
	DissolveCollider->SetSphereRadius(Radius, true);
}

void ADissolver::BorderSound() const{
	if(!IsValid(MainPlayer)){
		return;
	}

	const float DistanceFromPlayer = GetDistanceTo(MainPlayer) - Radius;

	const bool bSupDistance = DistanceFromPlayer > -MaxBorderDistance;

	const bool bInfDistance = DistanceFromPlayer < MaxBorderDistance;

	if(bSupDistance && bInfDistance){
		const float NormalizedValue = UKismetMathLibrary::NormalizeToRange(DistanceFromPlayer, -MaxBorderDistance, MaxBorderDistance);
		FMODAudio->SetParameter("border", NormalizedValue);
	}
}

void ADissolver::OnSwitchPhase(const EPhases NewPhase){
	switch (NewPhase){
		case EPhases::Infiltration:
			bAllowBorderSound = false;
			break;
		case EPhases::TowerDefense:
			bAllowBorderSound = true;
			break;
	}
}

void ADissolver::UpdateShaderFX(){
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPCDissolver, "Position1", GetActorLocation());

	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPCDissolver, "Bounds", GetActorScale());

	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPCDissolver, "Emissive", Emissive);

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPCDissolver, "SecondTilling", SecondTilling);

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPCDissolver, "RadiusScale", RadiusScale);

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPCDissolver, "Power", Power);

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPCDissolver, "TexRez", TexRez);

	UpdateRadius();
}

void ADissolver::PlayFX(){
	CurrentRadiusValue = Radius;

	if(CurrentRadiusValue != TargetRadiusValue){
		DissolveTimeline.PlayFromStart();
	}
}

void ADissolver::SetEnvironnementToMed(){
	Radius = MaxRadius;
	UpdateRadius();
}

void ADissolver::SetEnvironnementToTech(){
	Radius = 0;
	UpdateRadius();
}

void ADissolver::DissolveTo(const float TargetRadius){
	TargetRadiusValue = TargetRadius;
	PlayFX();
}

float ADissolver::GetRadius() const{
	return Radius;
}

void ADissolver::LerpRadius(float Value){
	Radius = FMath::Lerp(CurrentRadiusValue, TargetRadiusValue, Value);
	UpdateRadius();
}

void ADissolver::OnConstruction(const FTransform& Transform){
	Super::OnConstruction(Transform);

	UpdateShaderFX();
}

float ADissolver::GetMaxRadius() const{
	return MaxRadius;
}

