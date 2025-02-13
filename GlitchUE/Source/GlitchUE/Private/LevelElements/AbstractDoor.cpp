// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/AbstractDoor.h"
#include "Player/MainPlayer.h"
#include "AI/MainAICharacter.h"
#include "Helpers/FunctionsLibrary/UsefulFunctions.h"
#include "NavAreas/NavArea_Default.h"
#include "NavAreas/NavArea_Null.h"

AAbstractDoor::AAbstractDoor(){
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);
	RootComp->SetMobility(EComponentMobility::Static);

	MedFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Med Frame"));
	MedFrame->SetupAttachment(RootComp);
	MedFrame->SetCanEverAffectNavigation(false);
	MedFrame->SetMobility(EComponentMobility::Static);

	TechFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tech Frame"));
	TechFrame->SetupAttachment(RootComp);
	TechFrame->SetCanEverAffectNavigation(false);
	TechFrame->SetMobility(EComponentMobility::Static);

	MedDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Med Door"));
	MedDoor->SetupAttachment(MedFrame);
	MedDoor->SetCanEverAffectNavigation(false);

	TechDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tech Door"));
	TechDoor->SetupAttachment(TechFrame);
	TechDoor->SetCanEverAffectNavigation(false);

	DoorAudioComp = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Door Audio"));
	DoorAudioComp->SetupAttachment(RootComp);
	DoorAudioComp->SetRelativeLocation(FVector(0, 0, 150));

	static ConstructorHelpers::FObjectFinder<UFMODEvent> OpenSFX(TEXT("/Game/FMOD/Events/SFX/SFX_door_open"));
	check(OpenSFX.Succeeded());

	OpenDoorSFX = OpenSFX.Object;

	static ConstructorHelpers::FObjectFinder<UFMODEvent> CloseSFX(TEXT("/Game/FMOD/Events/SFX/SFX_door_close"));
	check(CloseSFX.Succeeded());

	CloseDoorSFX = CloseSFX.Object;

	NavLinkProxy = CreateDefaultSubobject<UChildActorComponent>(TEXT("Nav Link Proxy"));
	NavLinkProxy->SetupAttachment(RootComp);
	NavLinkProxy->SetMobility(EComponentMobility::Static);
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

	FWorldDelegates::OnWorldCleanup.AddUFunction(this, "OnCleanWorld");
}

void AAbstractDoor::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	OpenDoorTimeline.TickTimeline(DeltaTime);
}

void AAbstractDoor::OnCleanWorld(UWorld* World, bool bSessionEnded, bool bCleanupResources){
	World->GetTimerManager().ClearTimer(OpenDoorAITimerHandle);
	World->GetTimerManager().ClearTimer(ResetDoorAITimerHandle);
}

void AAbstractDoor::OpenAndCloseDoor(){
	bIsOpen ? CloseDoor() : OpenDoor();
}

void AAbstractDoor::OpenDoor(){
	OpenDoorTimeline.Play();

	DoorAudioComp->SetEvent(OpenDoorSFX);
	DoorAudioComp->Play();
}

void AAbstractDoor::CloseDoor(){
	OpenDoorTimeline.Reverse();

	DoorAudioComp->SetEvent(CloseDoorSFX);
	DoorAudioComp->Play();
}

void AAbstractDoor::OpenDoorUpdate(float Alpha){}

void AAbstractDoor::OpenDoorFinished(){
	bIsOpen = !bIsOpen;

	UpdateNavModiferParameters();

	UUsefulFunctions::MakeNoise(CurrentCharacterOpenningDoor, GetActorLocation(), OpenDoorNoiseRadius);
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

	GetWorld()->GetTimerManager().SetTimer(OpenDoorAITimerHandle, [&](){
		// if the ai dies
		if(IsValid(CurrentCharacterOpenningDoor)){
			Cast<AMainAICharacter>(CurrentCharacterOpenningDoor)->GetBlackBoard()->SetValueAsBool("DoingExternalActions", false);
		}

		GetWorld()->GetTimerManager().SetTimer(ResetDoorAITimerHandle, this, &AAbstractDoor::ResetReachLink, 1.0f, false);
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