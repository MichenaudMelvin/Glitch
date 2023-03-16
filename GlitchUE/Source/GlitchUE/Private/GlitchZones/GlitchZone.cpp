// Fill out your copyright notice in the Description page of Project Settings.


#include "GlitchZones/GlitchZone.h"
#include "Camera/CameraComponent.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayer.h"

AGlitchZone::AGlitchZone(){
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Engine/BasicShapes/Cube1"));
	check(Mesh.Succeeded());

	GetStaticMeshComponent()->SetStaticMesh(Mesh.Object);
	GetStaticMeshComponent()->SetWorldScale3D(FVector(2, 2, 2));

	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);

	TriggerBox->SetBoxExtent(FVector(50, 50,50));
}

void AGlitchZone::BeginPlay(){
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGlitchZone::EnterGlitchZone);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AGlitchZone::ExitGlitchZone);

	GameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AGlitchZone::EnterGlitchZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		MainPlayer = Cast<AMainPlayer>(OtherActor);
		MainPlayer->SetInGlitchZone(true);

		MainPlayer->EnableGlitchEffect(true, GlitchFadeTime);

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

		MainPlayer->EnableGlitchEffect(false, GlitchFadeTime);

		GetWorld()->GetTimerManager().ClearTimer(GlitchGaugeTimer);
		GetWorld()->GetTimerManager().ClearTimer(GlitchZoneTimer);
	}
}