// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPursuitDrone/PowerUpDrone.h"
#include "AI/AIPursuitDrone/PowerUpDroneData.h"
#include "Components/AudioComponent.h"
#include "Player/MainPlayerController.h"

APowerUpDrone::APowerUpDrone(){
	PrimaryActorTick.bCanEverTick = true;

	PivotPoint = CreateDefaultSubobject<USceneComponent>(TEXT("PivotPoint"));
	SetRootComponent(PivotPoint);

	DroneMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DroneMesh"));
	DroneMesh->SetupAttachment(RootComponent);

	DroneMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DroneMesh->SetCollisionResponseToAllChannels(ECR_Block);

	AudioComp->SetupAttachment(DroneMesh);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_ZeroToOneCurve"));
	check(Curve.Succeeded());

	ZeroToOneCurve = Curve.Object;
}

void APowerUpDrone::BeginPlay(){
	Super::BeginPlay();

	SetData(CurrentData);
	InteractableComp->AddInteractable(DroneMesh);

	FOnTimelineFloat UpdateEvent;

	UpdateEvent.BindDynamic(this, &APowerUpDrone::Spin);

	SpinTimeline.AddInterpFloat(ZeroToOneCurve, UpdateEvent);
	SpinTimeline.SetLooping(true);
	SpinTimeline.SetPlayRate(1/SpinSpeed);
}

void APowerUpDrone::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);

	SpinTimeline.TickTimeline(DeltaSeconds);
}

void APowerUpDrone::SetData(UPlacableActorData* NewData){
	Super::SetData(NewData);

	const UPowerUpDroneData* Data = Cast<UPowerUpDroneData>(CurrentData);
	SpinOffset = Data->SpinOffset;
	SpinSpeed = Data->SpinSpeed;
	InteriorRotationOffset = Data->InteriorRotationOffset;
}

void APowerUpDrone::SetMesh(){
	Super::SetMesh();

	DroneMesh->SetSkeletalMesh(Cast<USkeletalMesh>(CurrentData->MeshList[0]));
}

void APowerUpDrone::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	// super is used for selling objects
	// Super::Interact(MainPlayerController, MainPlayer);

	MainPlayer->SetPlacableActorData(CurrentData);
	MainPlayerController->BindConstructionMode();
}

void APowerUpDrone::BoostPlacable(APlacableActor* PlacableToBoost){
	Cast<IGlitchInterface>(PlacableToBoost)->ReciveGlitchUpgrade();

	DroneMesh->SetRelativeLocation(FVector(0, SpinOffset, 0));
	DroneMesh->SetRelativeRotation(FRotator(0, -180, InteriorRotationOffset));
	SpinTimeline.Play();
}

void APowerUpDrone::Spin(float Value){
	const float CurrentYaw = FMath::Lerp(Value, 0.0f, 360.0f);
	SetActorRotation(FRotator(0, CurrentYaw, 0));
}
