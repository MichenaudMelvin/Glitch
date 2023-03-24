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
#include "Kismet/KismetMaterialLibrary.h"
#include "PlacableObject/ConstructionZone.h"
#include "AI/AIPursuitDrone/PursuitDrone.h"

APlacableActor::APlacableActor(){
	PrimaryActorTick.bCanEverTick = false;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComp->SetupAttachment(RootComponent);

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));

	NavModifierComp = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifier"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_ZeroToOneCurve"));
	check(Curve.Succeeded());

	ZeroToOneCurve = Curve.Object;

	NavModifierComp->SetAreaClass(UNavArea_Obstacle::StaticClass());

	//static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> MPC(TEXT("/Game/VFX/Shaders/ConstructionNumeric/MPC_Construction"));
	//check(MPC.Succeeded());

	//AppearenceMaterialCollection = MPC.Object;

}

void APlacableActor::BeginPlay(){
	Super::BeginPlay();

	InteractableComp->OnInteract.AddDynamic(this, &APlacableActor::Interact);

	FOnTimelineFloat UpdateEvent;
	FOnTimelineEvent FinishedEvent;

	UpdateEvent.BindDynamic(this, &APlacableActor::FadeIn);
	FinishedEvent.BindDynamic(this, &APlacableActor::EndAppearence);

	FadeInAppearence.AddInterpFloat(ZeroToOneCurve, UpdateEvent);
	FadeInAppearence.SetTimelineFinishedFunc(FinishedEvent);

	FadeInAppearence.PlayFromStart();
}

void APlacableActor::Tick(float DeltaTime){
	FadeInAppearence.TickTimeline(DeltaTime);
}

void APlacableActor::SetMesh(){}

void APlacableActor::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	if (MainPlayerController->GetGameplayMode() == EGameplayMode::Destruction){
		SellObject(MainPlayer);
	}

	if(IsValid(CurrentDrone)){
		MainPlayer->SetCurrentDrone(CurrentDrone);
		CurrentDrone = nullptr;
		return;
	}

	if(IsValid(MainPlayer->GetCurrentDrone())){
		AddDrone(MainPlayer);
	}
}

void APlacableActor::SellObject(AMainPlayer* MainPlayer){
	MainPlayer->GiveGolds(CurrentData->Cost);
	AffectedConstructionZone->UnoccupiedSlot();

	Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->AddGlitch(GlitchGaugeValueOnDestruct);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	Destroy();
}

void APlacableActor::FadeIn(float Alpha){
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), AppearenceMaterialCollection, FName("Appearence"), Alpha);
}

void APlacableActor::AddDrone(AMainPlayer* MainPlayer){
	CurrentDrone = MainPlayer->GetCurrentDrone();
	MainPlayer->SetCurrentDrone(nullptr);

	CurrentDrone->AttachDrone(this, "");

	FVector TargetLocation = GetActorLocation();
	TargetLocation.Z += 100;

	CurrentDrone->SetActorLocation(TargetLocation);

	CurrentDrone->BoostPlacable();
}

void APlacableActor::SetObjectMaterial(UMaterialInterface* NewMaterial){}

void APlacableActor::EndAppearence_Implementation(){}

void APlacableActor::Attack_Implementation(){}

void APlacableActor::OnReachVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherComp->IsA(USightComponent::StaticClass())){
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

	SetMesh();

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
	// Ici set les upgrades dans les fonctions qui vont hériter

	UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *this->GetName());

	FTimerHandle TimerHandle;

	GetWorldTimerManager().SetTimer(TimerHandle, [&]() {
		ResetGlitchUpgrade();
	}, CurrentData->GlitchUpgradeDuration, false);
}

void APlacableActor::ResetGlitchUpgrade(){
	IGlitchInterface::ResetGlitchUpgrade();

	//reset à l'upgrade actuelle
	SetData(CurrentData);
}
