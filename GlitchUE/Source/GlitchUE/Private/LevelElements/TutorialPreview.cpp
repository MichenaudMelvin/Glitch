// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/TutorialPreview.h"
#include "Player/MainPlayer.h"

ATutorialPreview::ATutorialPreview(){
	PrimaryActorTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SetRootComponent(Spline);
	Spline->SetMobility(EComponentMobility::Static);

	Wisp = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("Will-o'-the-wisp"));
	Wisp->SetupAttachment(Spline);
	Wisp->bPlayOnLoad = false;

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> WispFX(TEXT("/Game/VFX/Particles/FX_Environment/Pk_FeuFollet"));
	check(WispFX.Succeeded());

	Wisp->SetEffect(WispFX.Object);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_ZeroToOneCurve"));
	check(Curve.Succeeded());

	FOnTimelineFloat UpdateEvent;
	FOnTimelineEvent FinishedEvent;

	UpdateEvent.BindDynamic(this, &ATutorialPreview::FollowSplineUpdate);
	FinishedEvent.BindDynamic(this, &ATutorialPreview::FollowSplineFinish);

	FollowTimeline.AddInterpFloat(Curve.Object, UpdateEvent);
	FollowTimeline.SetTimelineFinishedFunc(FinishedEvent);

	FirstBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("First Trigger Box"));
	FirstBoxComp->SetupAttachment(RootComponent);
	FirstBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FirstBoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	FirstBoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	FirstBoxComp->SetMobility(EComponentMobility::Static);
	FirstBoxComp->SetRelativeLocation(FVector(300, 0, 0));
	FirstBoxComp->SetBoxExtent(FVector(20, 80, 160));
	FirstBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ATutorialPreview::OverlapTrigger);

	SecondBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Second Trigger Box"));
	SecondBoxComp->SetupAttachment(RootComponent);
	SecondBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SecondBoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SecondBoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SecondBoxComp->SetMobility(EComponentMobility::Static);
	SecondBoxComp->SetRelativeLocation(FVector(-300, 0, 0));
	SecondBoxComp->SetBoxExtent(FVector(20, 80, 160));
	SecondBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ATutorialPreview::OverlapTrigger);
}

void ATutorialPreview::BeginPlay(){
	Super::BeginPlay();

	FollowTimeline.SetPlayRate(1/SplineSpeed);
	Wisp->SetWorldLocation(Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World));

	FWorldDelegates::OnWorldCleanup.AddUFunction(this, "OnCleanWorld");
}

void ATutorialPreview::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);

	FollowTimeline.TickTimeline(DeltaSeconds);
}

void ATutorialPreview::OnCleanWorld(UWorld* World, bool bSessionEnded, bool bCleanupResources){
	World->GetTimerManager().ClearTimer(LoopDelayTimerHandle);
}

void ATutorialPreview::FollowSpline(){
	FollowTimeline.PlayFromStart();
}

void ATutorialPreview::StopFollow(){
	Destroy();
}

void ATutorialPreview::FollowSplineUpdate(float Alpha){
	const float Distance = FMath::Lerp(0.0f, Spline->GetSplineLength(), Alpha);

	const FVector TargetLocation = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
	Wisp->SetRelativeLocation(TargetLocation);
}

void ATutorialPreview::FollowSplineFinish(){
	GetWorld()->GetTimerManager().SetTimer(LoopDelayTimerHandle, this, &ATutorialPreview::FollowSpline, LoopDelay, false);
}

void ATutorialPreview::OverlapTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		OverlappedComp->DestroyComponent();

		if(!bLoopStarted){
			Wisp->StartEmitter();
			OnReachFirstBox.Broadcast();
			FollowSpline();
			bLoopStarted = true;
		} else {
			OnReachSecondBox.Broadcast();
			StopFollow();
		}

	}
}
