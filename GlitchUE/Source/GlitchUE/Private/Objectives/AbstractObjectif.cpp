// Fill out your copyright notice in the Description page of Project Settings.

#include "Objectives/AbstractObjectif.h"
#include "PopcornFXEmitterComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavAreas/NavArea_Obstacle.h"

AAbstractObjectif::AAbstractObjectif(){
	PrimaryActorTick.bCanEverTick = false;

	MeshObjectif = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshObjectif"));

	SetRootComponent(MeshObjectif);

	MeshObjectif->SetCollisionResponseToAllChannels(ECR_Block);
	MeshObjectif->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	MeshObjectif->SetCanEverAffectNavigation(false);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

	ActivableComp = CreateDefaultSubobject<UActivableComponent>(TEXT("Activable"));

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));

	NavModifier = CreateDefaultSubobject<UBoxComponent>(TEXT("Nav Modifier"));
	NavModifier->SetupAttachment(RootComponent);

	NavModifier->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	NavModifier->SetCollisionResponseToAllChannels(ECR_Ignore);
	NavModifier->AreaClass = UNavArea_Obstacle::StaticClass();
	NavModifier->bDynamicObstacle = true;

	static ConstructorHelpers::FObjectFinder<UFMODEvent> SFXActivation(TEXT("/Game/FMOD/Events/SFX/SFX_Free_Interaction"));
	check(SFXActivation.Succeeded());

	ActivationSFX = SFXActivation.Object;
}

void AAbstractObjectif::BeginPlay(){
	Super::BeginPlay();

	ActivableComp->OnActivated.AddDynamic(this, &AAbstractObjectif::ActiveObjectif);
	ActivableComp->OnDesactivated.AddDynamic(this, &AAbstractObjectif::DesactivateObjectif);

	InteractableComp->OnInteract.AddDynamic(this, &AAbstractObjectif::Interact);

	HealthComp->OnReciveDamages.AddDynamic(this, &AAbstractObjectif::TakeDamages);
	HealthComp->OnHealthNull.AddDynamic(this, &AAbstractObjectif::HealthNull);

	if(UGameplayStatics::GetGameMode(GetWorld())->IsA(AGlitchUEGameMode::StaticClass())){
		GameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		GameMode->OnSwitchPhases.AddDynamic(this, &AAbstractObjectif::OnSwitchPhases);
	}
}

void AAbstractObjectif::ActiveObjectif(){}

void AAbstractObjectif::DesactivateObjectif(){}

void AAbstractObjectif::OnSwitchPhases(EPhases CurrentPhase){}

void AAbstractObjectif::TakeDamages(){}

void AAbstractObjectif::HealthNull(){}

void AAbstractObjectif::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){}

void AAbstractObjectif::DestroyFX(UPopcornFXEmitterComponent* EmitterComponent, FVector Location, FVector Rotation){
	EmitterComponent->DestroyComponent();
}

UActivableComponent* AAbstractObjectif::GetActivableComp()  const{
	return ActivableComp;
}

UHealthComponent* AAbstractObjectif::GetHealthComp() const{
	return HealthComp;
}
