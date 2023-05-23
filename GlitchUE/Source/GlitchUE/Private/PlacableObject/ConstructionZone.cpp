// Fill out your copyright notice in the Description page of Project Settings.

#include "PlacableObject/ConstructionZone.h"
#include "PopcornFXEmitterComponent.h"
#include "PopcornFXFunctions.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "Player/MainPlayerController.h"

AConstructionZone::AConstructionZone() {
	PrimaryActorTick.bCanEverTick = false;

	ActivableComp = CreateDefaultSubobject<UActivableComponent>(TEXT("Activable"));
	ActivableComp->OnActivated.AddDynamic(this, &AConstructionZone::ActiveObjectif);
	ActivableComp->OnDesactivated.AddDynamic(this, &AConstructionZone::DesactivateObjectif);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MedSkelMesh(TEXT("/Game/Meshs/Turrets/Socles/SK_MED_Socle"));
	check(MedSkelMesh.Succeeded());

	GetSkeletalMeshComponent()->SetSkeletalMesh(MedSkelMesh.Object);
	GetSkeletalMeshComponent()->SetCanEverAffectNavigation(true);

	TechMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TechMesh"));
	TechMesh->SetupAttachment(GetSkeletalMeshComponent());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TechSkelMesh(TEXT("/Game/Meshs/Turrets/Socles/SK_TECH_Socle"));
	check(TechSkelMesh.Succeeded());

	TechMesh->SetSkeletalMesh(TechSkelMesh.Object);
	TechMesh->SetCanEverAffectNavigation(true);

	NavObstacle = CreateDefaultSubobject<UBoxComponent>("Nav Obstacle");
	NavObstacle->SetupAttachment(RootComponent);

	NavObstacle->SetCollisionResponseToAllChannels(ECR_Ignore);
	NavObstacle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	NavObstacle->SetGenerateOverlapEvents(false);

	NavObstacle->AreaClass = UNavArea_Obstacle::StaticClass();
	NavObstacle->bDynamicObstacle = true;

	NavObstacle->SetBoxExtent(FVector(60, 60, 10));
	NavObstacle->SetRelativeLocation(FVector(0, 0, 100));

	NavObstacle->SetVisibility(false);

	InitialState = EState::Desactivated;

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("ConstructionZone Interaction"));
	InteractableComponent->OnInteract.AddDynamic(this, &AConstructionZone::Interact);

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> FX(TEXT("/Game/VFX/Particles/FX_Environment/Pk_ConstructionZone"));
	check(FX.Succeeded());

	ConstructionEffect = FX.Object;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> ActivAnim(TEXT("/Game/Meshs/Turrets/Socles/AS_Socle"));
	check(ActivAnim.Succeeded());

	ActivationAnim = ActivAnim.Object;
}

USkeletalMeshComponent* AConstructionZone::GetTechMesh() const{
	return TechMesh;
}

void AConstructionZone::BeginPlay(){
	Super::BeginPlay();

	FVector FXLocation = GetActorLocation();
	FXLocation.Z += 5;
	ConstructionFXEmitter = UPopcornFXFunctions::SpawnEmitterAtLocation(GetWorld(), ConstructionEffect, "PopcornFX_DefaultScene", FXLocation, FRotator::ZeroRotator, false, false);

	GameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->OnSwitchPhases.AddDynamic(this, &AConstructionZone::SwitchPhases);

	switch (InitialState){
	case EState::Activated:
		ActivableComp->ActivateObject();
		break;
	case EState::Desactivated:
		ActivableComp->DesactivateObject();
		break;
	}
}

void AConstructionZone::ActiveObjectif(){
	ConstructionFXEmitter->StartEmitter();
	GetSkeletalMeshComponent()->PlayAnimation(ActivationAnim, false);
	TechMesh->PlayAnimation(ActivationAnim, false);

	if(GameMode->GetPhases() == EPhases::TowerDefense){
		InteractableComponent->AddInteractable(GetSkeletalMeshComponent());
		InteractableComponent->AddInteractable(TechMesh);
	}
}

void AConstructionZone::DesactivateObjectif(){
	ConstructionFXEmitter->StopEmitter();

	GetSkeletalMeshComponent()->SetPlayRate(-1);
	GetSkeletalMeshComponent()->Play(false);
	TechMesh->SetPlayRate(-1);
	TechMesh->Play(false);

	if(IsValid(UnitInZone)){
		FOnTimelineEvent FinishEvent;
		FinishEvent.BindDynamic(UnitInZone, &APlacableActor::CallDestroy);
		UnitInZone->Appear(true, FinishEvent);
	}

	if(GameMode->GetPhases() == EPhases::TowerDefense){
		InteractableComponent->RemoveInteractable(GetSkeletalMeshComponent());
		InteractableComponent->RemoveInteractable(TechMesh);
	}
}

void AConstructionZone::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	MainPlayer->OpenConstructionZone(this);
	MainPlayerController->OpenWheel();
}

void AConstructionZone::SwitchPhases(EPhases NewPhases){
	if(NewPhases == EPhases::TowerDefense && ActivableComp->IsActivated()){
		InteractableComponent->AddInteractable(GetSkeletalMeshComponent());
		InteractableComponent->AddInteractable(TechMesh);
	}
}

void AConstructionZone::OccupiedSlot(APlacableActor* NewUnit){
	UnitInZone = NewUnit;
	UnitInZone->SetConstructionZone(this);
	ConstructionFXEmitter->StopEmitter();
	NavObstacle->SetRelativeLocation(FVector(0, 0, 0));
}

void AConstructionZone::UnoccupiedSlot(){
	UnitInZone = nullptr;
	if(ActivableComp->IsActivated()){
		ConstructionFXEmitter->StartEmitter();
	}

	NavObstacle->SetRelativeLocation(FVector(0, 0, 100));
}

bool AConstructionZone::IsSlotOccupied() const{
	return IsValid(UnitInZone);
}

UActivableComponent* AConstructionZone::GetActivableComp(){
	return ActivableComp;
}

void AConstructionZone::DestroyCurrentUnit(){
	UnitInZone->SellObject();
}

#if WITH_EDITORONLY_DATA
void AConstructionZone::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FVector SnappedLocation = UKismetMathLibrary::Vector_SnappedToGrid(GetActorLocation(), 200);
	SnappedLocation.Z = GetActorLocation().Z;

	SetActorLocation(SnappedLocation);
}
#endif
