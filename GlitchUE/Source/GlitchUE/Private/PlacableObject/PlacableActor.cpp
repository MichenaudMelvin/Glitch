// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableObject/PlacableActor.h"
#include "..\..\Public\PlacableObject\PlacableActor.h"

#include "PopcornFXFunctions.h"
#include "Components/InteractableComponent.h"
#include "Player/MainPlayer.h"
#include "Player/MainPlayerController.h"
#include "Components/TimelineComponent.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "PlacableObject/ConstructionZone.h"

APlacableActor::APlacableActor(){
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComp->SetupAttachment(BaseMesh);

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
	InteractableComp->AddInteractable(BaseMesh);

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

void APlacableActor::SetMesh() {
	BaseMesh->SetStaticMesh(Cast<UStaticMesh>(CurrentData->MeshList[0]));
}

void APlacableActor::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	if (MainPlayerController->GetGameplayMode() == EGameplayMode::CPF_Destruction) {
		SellObject(MainPlayer);
	}
}

void APlacableActor::SellObject(AMainPlayer* MainPlayer){
	MainPlayer->GiveGolds(CurrentData->Cost);
	AffectedConstructionZone->UnoccupiedSlot();
	Destroy();
}

void APlacableActor::FadeIn(float Alpha){
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), AppearenceMaterialCollection, FName("Appearence"), Alpha);
}

void APlacableActor::SetObjectMaterial(UMaterialInterface* NewMaterial){
	BaseMesh->SetMaterial(0, NewMaterial);
}

void APlacableActor::EndAppearence_Implementation(){}

void APlacableActor::SetData(UPlacableActorData* NewData){
	CurrentData = NewData;
	Name = CurrentData->Name;
	AttackRange = CurrentData->AttackRange;
	SetMesh();

	if(AttackFX == nullptr){
		AttackFX = UPopcornFXFunctions::SpawnEmitterAtLocation(GetWorld(), CurrentData->AttackFX, "PopcornFX_DefaultScene", FVector::ZeroVector, FRotator::ZeroRotator, false, false);
	}
}

void APlacableActor::SetConstructionZone(AConstructionZone* NewConstructionZone) {
	AffectedConstructionZone = NewConstructionZone;
}

void APlacableActor::Upgrade(){
	SetData(CurrentData->NextUpgrade);
}

void APlacableActor::GlitchUpgrade(){
	// Ici set les upgrades dans les fonctions qui vont hériter

	FTimerHandle TimerHandle;

	GetWorldTimerManager().SetTimer(TimerHandle, [&]() {
		//reset à l'upgrade actuelle
		SetData(CurrentData);
	}, CurrentData->GlitchUpgradeDuration, false);
}