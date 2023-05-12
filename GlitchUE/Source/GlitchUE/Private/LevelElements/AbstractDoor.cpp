// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/AbstractDoor.h"
#include "Player/MainPlayer.h"
#include "AI/MainAICharacter.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "NavAreas/NavArea_Default.h"
#include "NavAreas/NavArea_Null.h"

AAbstractDoor::AAbstractDoor(){
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	MedFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Med Frame"));
	MedFrame->SetupAttachment(RootComp);
	MedFrame->SetCanEverAffectNavigation(false);

	TechFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tech Frame"));
	TechFrame->SetupAttachment(RootComp);
	TechFrame->SetCanEverAffectNavigation(false);

	MedDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Med Door"));
	MedDoor->SetupAttachment(MedFrame);
	MedDoor->SetCanEverAffectNavigation(false);

	TechDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tech Door"));
	TechDoor->SetupAttachment(TechFrame);
	TechDoor->SetCanEverAffectNavigation(false);

	NavLinkProxy = CreateDefaultSubobject<UChildActorComponent>(TEXT("Nav Link Proxy"));
	NavLinkProxy->SetupAttachment(RootComp);
	// make the game crash, needs to be set in blueprint
	//NavLinkProxy->SetChildActorClass(ANavLinkProxy::StaticClass());

	NavModifier = CreateDefaultSubobject<UNavModifierComponent>(TEXT("Nav Modifier"));
	NavModifier->SetAreaClass(UNavArea_Null::StaticClass());

	DoorInteraction = CreateDefaultSubobject<UInteractableComponent>(TEXT("Door Interaction"));
	DoorInteraction->OnInteract.AddDynamic(this, &AAbstractDoor::OnInteract);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_ZeroToOneCurve"));
	check(Curve.Succeeded());

	ZeroToOneCurve = Curve.Object;

	FOnTimelineFloat UpdateEvent;
	FOnTimelineEvent FinishedEvent;

	UpdateEvent.BindDynamic(this, &AAbstractDoor::OpenDoorUpdate);
	FinishedEvent.BindDynamic(this, &AAbstractDoor::OpenDoorFinished),

	OpenDoorTimeline.AddInterpFloat(ZeroToOneCurve, UpdateEvent);
	OpenDoorTimeline.SetTimelineFinishedFunc(FinishedEvent);
}

void AAbstractDoor::BeginPlay(){
	Super::BeginPlay();

	NavLink = Cast<ANavLinkProxy>(NavLinkProxy->GetChildActor());

	DoorInteraction->AddInteractable(MedDoor);
	DoorInteraction->AddInteractable(TechDoor);

	OpenDoorTimeline.SetPlayRate(1/OpenDoorTime);
}

void AAbstractDoor::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	OpenDoorTimeline.TickTimeline(DeltaTime);
}

void AAbstractDoor::OpenAndCloseDoor(){
	bIsOpen ? CloseDoor() : OpenDoor();
}

void AAbstractDoor::OpenDoor(){
	OpenDoorTimeline.Play();
}

void AAbstractDoor::CloseDoor(){
	OpenDoorTimeline.Reverse();
}

void AAbstractDoor::OpenDoorUpdate(float Alpha){}

void AAbstractDoor::OpenDoorFinished(){
	bIsOpen = !bIsOpen;

	UpdateNavModiferParameters();

	UUsefullFunctions::MakeNoise(CurrentCharacterOpenningDoor, GetActorLocation(), OpenDoorNoiseRadius);
}

void AAbstractDoor::OnAIReachLink(AActor* MovingActor, const FVector& DestinationPoint){
	if(bIsLinkReached){
		return;
	}

	bIsLinkReached = true;
	AMainAICharacter* AICharacter = Cast<AMainAICharacter>(MovingActor);

	AICharacter->GetBlackBoard()->SetValueAsBool("DoingExternalActions", true);

	if(bIsOpen){
		ResetReachLink();
		return;
	}

	CurrentCharacterOpenningDoor = AICharacter;
	OpenDoor();

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){
		Cast<AMainAICharacter>(CurrentCharacterOpenningDoor)->GetBlackBoard()->SetValueAsBool("DoingExternalActions", true);

		FTimerHandle ResetTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(ResetTimerHandle, this, &AAbstractDoor::ResetReachLink, 1.0f, false);
	}, OpenDoorTimeline.GetTimelineLength() / OpenDoorTimeline.GetPlayRate(), false);
}

void AAbstractDoor::ResetReachLink(){
	bIsLinkReached = false;
}

void AAbstractDoor::OnInteract(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	CurrentCharacterOpenningDoor = MainPlayer;

	if(!OpenDoorTimeline.IsPlaying()){
		OpenAndCloseDoor();
	}
}

void AAbstractDoor::OpenDoorEditor(){}

void AAbstractDoor::CloseDoorEditor(){}

void AAbstractDoor::UpdateNavModiferParameters() const{
	NavLink->SetSmartLinkEnabled(!bIsOpen);
	NavModifier->SetAreaClass(bIsOpen ? UNavArea_Default::StaticClass() : UNavArea_Null::StaticClass());
}