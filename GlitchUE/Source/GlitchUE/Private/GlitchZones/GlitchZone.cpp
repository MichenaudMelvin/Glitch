// Fill out your copyright notice in the Description page of Project Settings.


#include "GlitchZones/GlitchZone.h"
#include "Camera/CameraComponent.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Player/MainPlayer.h"

AGlitchZone::AGlitchZone(){
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Engine/BasicShapes/Cube1"));
	check(Mesh.Succeeded());

	GetStaticMeshComponent()->SetStaticMesh(Mesh.Object);
	GetStaticMeshComponent()->SetWorldScale3D(FVector(2, 2, 2));

	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);

	TriggerBox->SetBoxExtent(FVector(50, 50,50));

	PostProcessMaterialUI.Weight = 1;

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_ZeroToOneCurve"));
	check(Curve.Succeeded());

	ZeroToOneCurve = Curve.Object;
}

void AGlitchZone::BeginPlay(){
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGlitchZone::EnterGlitchZone);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AGlitchZone::ExitGlitchZone);

	GameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	FOnTimelineFloat UpdateEvent;
	FOnTimelineEvent FinishEvent;

	UpdateEvent.BindDynamic(this, &AGlitchZone::FadeInGlitchEffect);
	FinishEvent.BindDynamic(this, &AGlitchZone::EndFadeIn);

	FadeInGlitchEffectTimeline.AddInterpFloat(ZeroToOneCurve, UpdateEvent);
	FadeInGlitchEffectTimeline.SetTimelineFinishedFunc(FinishEvent);

	FadeInGlitchEffectTimeline.SetPlayRate(1/GlitchFadeTime);
}

void AGlitchZone::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);

	FadeInGlitchEffectTimeline.TickTimeline(DeltaSeconds);
}

void AGlitchZone::EnterGlitchZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		MainPlayer = Cast<AMainPlayer>(OtherActor);
		MainPlayer->SetInGlitchZone(true);

		EnableGlitchEffect(true);

		GetWorld()->GetTimerManager().SetTimer(GlitchGaugeTimer, [&]() {
			GameMode->AddGlitch(GlitchGaugeValueToAddEveryTick);
		}, GlitchGaugeTick, true);

		if(GameMode->GetLevelState() == ELevelState::Alerted){
			GetWorld()->GetTimerManager().SetTimer(GlitchZoneTimer, [&]() {
				GameMode->SetLevelState(ELevelState::Normal);
			}, ResetLevelStateDuration, false);
		}
	}
}

void AGlitchZone::ExitGlitchZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		MainPlayer = Cast<AMainPlayer>(OtherActor);
		MainPlayer->SetInGlitchZone(false);

		EnableGlitchEffect(false);

		GetWorld()->GetTimerManager().ClearTimer(GlitchGaugeTimer);
		GetWorld()->GetTimerManager().ClearTimer(GlitchZoneTimer);
	}
}

void AGlitchZone::EnableGlitchEffect(const bool bEnable){
	if(bEnable){
		FPostProcessSettings NewSettings;
		NewSettings.WeightedBlendables.Array.Add(PostProcessMaterialUI);
		MainPlayer->GetFollowCamera()->PostProcessSettings = NewSettings;
	}

	bEnable ? FadeInGlitchEffectTimeline.Play() : FadeInGlitchEffectTimeline.Reverse();
}

void AGlitchZone::FadeInGlitchEffect(float Value){
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), GlitchMPC, "ApparitionPostProcess", Value);
}

void AGlitchZone::EndFadeIn(){
	const float GlitchEffectValue = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), GlitchMPC, "ApparitionPostProcess");

	if (GlitchEffectValue == 0){
		FPostProcessSettings NewSettings;
		NewSettings.WeightedBlendables.Array.Empty();
		MainPlayer->GetFollowCamera()->PostProcessSettings = NewSettings;
	}
}
