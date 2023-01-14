// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableObject/Turret.h"
#include "Components/InteractableComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI/MainAICharacter.h"
#include "PlacableObject/TurretData.h"

ATurret::ATurret() {
	TurretPillar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pillar"));
	TurretPillar->SetupAttachment(BaseMesh);
	
	//TurretHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	//TurretHead->SetupAttachment(TurretPillar);

	TurretVision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Vision"));
	TurretVision->SetupAttachment(BaseMesh);
	TurretVision->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	TurretVision->bHiddenInGame = true;

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/ZeroToOneCurve"));
	check(Curve.Succeeded());

	ZeroToOneCurve = Curve.Object;
}

void ATurret::BeginPlay(){
	Super::BeginPlay();

	InteractableComp->AddInteractable(TurretPillar);
	//InteractableComp->AddInteractable(TurretHead);

	FOnTimelineFloat UpdateEvent;
	UpdateEvent.BindUFunction(this, FName{ TEXT("RotateToTarget") });
	RotateTimeline.AddInterpFloat(ZeroToOneCurve, UpdateEvent);
}

void ATurret::Tick(float deltaTime){
	Super::Tick(deltaTime);

	RotateTimeline.TickTimeline(deltaTime);
}

void ATurret::LookAtTarget(AActor* Target){
	CurrentTarget = Target;
	CurrentYawRotation = TurretPillar->GetComponentRotation().Yaw;
	RotateTimeline.PlayFromStart();
}

void ATurret::RotateToTarget(float Alpha){
	if (!IsValid(CurrentTarget)){
		RotateTimeline.Stop();
		return;
	}

	AILookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTarget->GetActorLocation());
	FRotator TargetRotator;

	TargetRotator.Yaw = FMath::Lerp(CurrentYawRotation, AILookAtRotation.Yaw, Alpha);

	TurretPillar->SetWorldRotation(TargetRotator);
}

void ATurret::GlitchUpgrade(){
	Damages = Cast<UTurretData>(CurrentData)->UpgradedGlitchDamages;
	
	Super::GlitchUpgrade();
}

void ATurret::SetMesh(){
	Super::SetMesh();

	TurretPillar->SetStaticMesh(Cast<UStaticMesh>(CurrentData->MeshList[1]));
}

void ATurret::SetData(UPlacableActorData* NewData){
	Super::SetData(NewData);
	
	UTurretData* Data = Cast<UTurretData>(NewData);
	Damages = Data->Damages;
	FireRate = Data->FireRate;
}
