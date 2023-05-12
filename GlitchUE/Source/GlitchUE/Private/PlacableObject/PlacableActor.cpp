// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableObject/PlacableActor.h"
#include "AI/MainAICharacter.h"
#include "PopcornFXFunctions.h"
#include "Components/AudioComponent.h"
#include "Components/InteractableComponent.h"
#include "Player/MainPlayer.h"
#include "Player/MainPlayerController.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "PlacableObject/ConstructionZone.h"
#include "AI/AIPursuitDrone/PursuitDrone.h"

APlacableActor::APlacableActor(){
	PrimaryActorTick.bCanEverTick = true;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComp->SetupAttachment(RootComponent);

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_ZeroToOneCurve"));
	check(Curve.Succeeded());

	ZeroToOneCurve = Curve.Object;
}

void APlacableActor::BeginPlay(){
	Super::BeginPlay();

	InteractableComp->OnInteract.AddDynamic(this, &APlacableActor::Interact);

	FOnTimelineFloat UpdateEvent;

	UpdateEvent.BindDynamic(this, &APlacableActor::FadeIn);

	FadeInAppearance.AddInterpFloat(ZeroToOneCurve, UpdateEvent);
	FadeInAppearance.SetPlayRate(1/AppearanceTime);
}

void APlacableActor::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	FadeInAppearance.TickTimeline(DeltaTime);
}

void APlacableActor::Destroyed(){
	if(IsValid(CurrentDrone)){
		CurrentDrone->DisableSpinBehavior();
	}

	AffectedConstructionZone->UnoccupiedSlot();

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	Super::Destroyed();
}

void APlacableActor::SetMesh(){}

void APlacableActor::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	if(IsValid(CurrentDrone) && IsValid(MainPlayer->GetCurrentDrone())){
		// à voir ce cas plus tard mais pour l'instant comme ça pour éviter des problèmes
		return;
	}

	bool bIsSelling = false;

	if (MainPlayerController->GetGameplayMode() == EGameplayMode::Destruction){
		MainPlayer->UpdateGolds(CurrentData->Cost, EGoldsUpdateMethod::ReceiveGolds);

		FOnTimelineEvent FinishEvent;
		FinishEvent.BindDynamic(this, &APlacableActor::SellObject);

		Appear(true, FinishEvent);
		bIsSelling = true;
	}

	if(IsValid(CurrentDrone)){
		RemoveDrone(MainPlayer);
		return;
	}

	if(bIsSelling){
		return;
	}

	if(IsValid(MainPlayer->GetCurrentDrone())){
		AddDrone(MainPlayer);
	}
}

void APlacableActor::SellObject(){
	Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->AddGlitch(GlitchGaugeValueOnDestruct);

	Destroy();
}

void APlacableActor::Appear(const bool ReverseEffect, const FOnTimelineEvent AppearFinishEvent){
	bIsAppearing = true;

	WireframeMesh = Cast<UStaticMeshComponent>(AddComponentByClass(UStaticMeshComponent::StaticClass(), true, GetActorTransform(), false));

	WireframeMesh->SetStaticMesh(CurrentData->FullMesh);

	for(int i = 0; i < WireframeMesh->GetNumMaterials(); i++){
		WireframeMesh->SetMaterial(i, WireframeMaterial);
	}

	ReverseEffect ? FadeInAppearance.Reverse() : FadeInAppearance.Play();

	FadeInAppearance.SetTimelineFinishedFunc(AppearFinishEvent);
}

void APlacableActor::SetNexus(ANexus* NewNexus){
	Nexus = NewNexus;
}

void APlacableActor::FadeIn(float Alpha){}

void APlacableActor::EndAppearance(){
	bIsAppearing = false;

	WireframeMesh->DestroyComponent();

	SetMesh();
}

void APlacableActor::AttachDroneToPlacable(APursuitDrone* NewDrone){
	CurrentDrone = NewDrone;

	AttackRate -= CurrentData->BoostDroneAttackRate;
	Damages += CurrentData->BoostDroneDamages;
	AttackRange += CurrentData->BoostDroneAttackRange;

	CurrentDrone->AttachDrone(this, "");

	FVector TargetLocation = GetActorLocation();
	TargetLocation.Z += 100;

	CurrentDrone->SetActorLocation(TargetLocation);
}


void APlacableActor::AddDrone(AMainPlayer* MainPlayer){
	AttachDroneToPlacable(MainPlayer->GetCurrentDrone());
	MainPlayer->SetCurrentDrone(nullptr);
}

void APlacableActor::RemoveDrone(AMainPlayer* MainPlayer){
	AttackRate += CurrentData->BoostDroneAttackRate;
	Damages -= CurrentData->BoostDroneDamages;
	AttackRange -= CurrentData->BoostDroneAttackRange;

	if(IsValid(MainPlayer)){
		MainPlayer->SetCurrentDrone(CurrentDrone);
	}

	CurrentDrone = nullptr;
}

void APlacableActor::SetObjectMaterial(UMaterialInterface* NewMaterial){}

void APlacableActor::Attack_Implementation(){}

void APlacableActor::OnReachVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherComp->IsA(USightComponent::StaticClass())){
		return;
	}

	if(OtherActor->IsA(APursuitDrone::StaticClass())){
		return;
	}

	if(OtherActor->IsA(AMainAICharacter::StaticClass())){
		AIList.Add(Cast<AMainAICharacter>(OtherActor));
	}
}

void APlacableActor::OnLeaveVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	if(OtherComp->IsA(USightComponent::StaticClass())){
		return;
	}

	if(OtherActor->IsA(APursuitDrone::StaticClass())){
		return;
	}

	if(OtherActor->IsA(AMainAICharacter::StaticClass())){
		AIList.Remove(Cast<AMainAICharacter>(OtherActor));
	}
}

void APlacableActor::SetData(UPlacableActorData* NewData){
	CurrentData = NewData;
	Name = CurrentData->Name;
	AttackRange = CurrentData->AttackRange;
	Damages = CurrentData->Damages;
	AttackAnimation = CurrentData->AttackAnimation;
	IdleAnimation = CurrentData->IdleAnimation;
	GlitchGaugeValueOnDestruct = CurrentData->GlitchGaugeValueOnDestruct;

	FOnTimelineEvent FinishEvent;
	FinishEvent.BindDynamic(this, &APlacableActor::EndAppearance);

	Appear(false, FinishEvent);

	if(AttackFX == nullptr){
		AttackFX = UPopcornFXFunctions::SpawnEmitterAtLocation(GetWorld(), CurrentData->AttackFX, "PopcornFX_DefaultScene", GetActorLocation() + CurrentData->AttackFXOffset, FRotator::ZeroRotator, false, false);
	}
}

void APlacableActor::SetConstructionZone(AConstructionZone* NewConstructionZone) {
	AffectedConstructionZone = NewConstructionZone;
}

void APlacableActor::Upgrade(){
	SetData(CurrentData->NextUpgrade);
}

void APlacableActor::ReceiveGlitchUpgrade(){
	IGlitchInterface::ReceiveGlitchUpgrade();

	AttackRate -= CurrentData->GlitchAttackRate;
	Damages += CurrentData->GlitchDamages;
	AttackRange += CurrentData->GlitchAttackRange;

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlacableActor::ResetGlitchUpgrade, CurrentData->GlitchUpgradeDuration, false);
}

void APlacableActor::ResetGlitchUpgrade(){
	IGlitchInterface::ResetGlitchUpgrade();

	AttackRate += CurrentData->GlitchAttackRate;
	Damages -= CurrentData->GlitchDamages;
	AttackRange -= CurrentData->GlitchAttackRange;
}

FPlacableActorSaveData APlacableActor::SavePlacable(){
	FPlacableActorSaveData CurrentSaveData;

	CurrentSaveData.PlacableActorClass = GetClass();
	CurrentSaveData.ActorTransform = GetActorTransform();
	CurrentSaveData.CurrentPlacableData = CurrentData;

	CurrentSaveData.bHasDrone = IsValid(CurrentDrone);

	if(CurrentSaveData.bHasDrone){
		CurrentSaveData.DroneName = CurrentDrone->GetName();
	}

	return CurrentSaveData;
}

void APlacableActor::InitializePlacable(const FPlacableActorSaveData NewData, TArray<AActor*> PursuitDroneList){
	FHitResult HitResult;
	const FVector StartLocation = GetActorLocation();
	FVector EndLocation = GetActorLocation();
	EndLocation.Z = - 5;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLocation, EndLocation, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	Cast<AConstructionZone>(HitResult.Actor)->OccupiedSlot(this);

	SetData(NewData.CurrentPlacableData);

	if(!NewData.bHasDrone){
		return;
	}

	for(int i = 0; i < PursuitDroneList.Num(); i++){
		if(PursuitDroneList[i]->GetName() == NewData.DroneName){
			AttachDroneToPlacable(Cast<APursuitDrone>(PursuitDroneList[i]));
			break;
		}
	}
}

void APlacableActor::CallDestroy(){
	Destroy();
}
