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

	NavModifierComp = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifier"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_ZeroToOneCurve"));
	check(Curve.Succeeded());

	ZeroToOneCurve = Curve.Object;

	NavModifierComp->SetAreaClass(UNavArea_Obstacle::StaticClass());
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
	FadeInAppearance.TickTimeline(DeltaTime);
}

void APlacableActor::SetMesh(){}

void APlacableActor::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	if(IsValid(CurrentDrone) && IsValid(MainPlayer->GetCurrentDrone())){
		// à voir ce cas plus tard mais pour l'instant comme ça pour éviter des problèmes
		return;
	}

	bool bIsSelling = false;

	if (MainPlayerController->GetGameplayMode() == EGameplayMode::Destruction){
		MainPlayer->GiveGolds(CurrentData->Cost);
		Appear(true);
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
	AffectedConstructionZone->UnoccupiedSlot();

	Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->AddGlitch(GlitchGaugeValueOnDestruct);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	Destroy();
}

void APlacableActor::Appear(const bool ReverseEffect){
	WireframeMesh = Cast<UStaticMeshComponent>(AddComponentByClass(UStaticMeshComponent::StaticClass(), true, GetActorTransform(), false));

	WireframeMesh->SetStaticMesh(CurrentData->FullMesh);

	for(int i = 0; i < WireframeMesh->GetNumMaterials(); i++){
		WireframeMesh->SetMaterial(i, WireframeMaterial);
	}

	FOnTimelineEvent FinishFunc;

	if(ReverseEffect){
		FinishFunc.BindDynamic(this, &APlacableActor::SellObject);
		FadeInAppearance.Reverse();
	} else{
		FinishFunc.BindDynamic(this, &APlacableActor::EndAppearance);
		FadeInAppearance.Play();
	}

	FadeInAppearance.SetTimelineFinishedFunc(FinishFunc);
}

void APlacableActor::FadeIn(float Alpha){}

void APlacableActor::EndAppearance(){
	WireframeMesh->DestroyComponent();

	SetMesh();
}

void APlacableActor::AddDrone(AMainPlayer* MainPlayer){
	CurrentDrone = MainPlayer->GetCurrentDrone();
	MainPlayer->SetCurrentDrone(nullptr);

	AttackRate -= CurrentData->BoostDroneAttackRate;
	Damages += CurrentData->BoostDroneDamages;
	AttackRange += CurrentData->BoostDroneAttackRange;

	CurrentDrone->AttachDrone(this, "");

	FVector TargetLocation = GetActorLocation();
	TargetLocation.Z += 100;

	CurrentDrone->SetActorLocation(TargetLocation);

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

	Appear();

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
