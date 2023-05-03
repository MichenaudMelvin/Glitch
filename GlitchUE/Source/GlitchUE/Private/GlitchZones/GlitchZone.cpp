// Fill out your copyright notice in the Description page of Project Settings.


#include "GlitchZones/GlitchZone.h"
#include "Audio/AudioManager.h"
#include "Camera/CameraComponent.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "Player/MainPlayer.h"

AGlitchZone::AGlitchZone(){
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Engine/BasicShapes/Cube1"));
	check(Mesh.Succeeded());

	GetStaticMeshComponent()->SetStaticMesh(Mesh.Object);
	GetStaticMeshComponent()->SetWorldScale3D(FVector(2, 2, 2));

	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);
	GetStaticMeshComponent()->SetGenerateOverlapEvents(true);

#if WITH_EDITORONLY_DATA

	Billboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(RootComponent);
	Billboard->bIsEditorOnly = true;

#endif

	NavModifierComp = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifier"));
	NavModifierComp->AreaClass = UNavArea_Obstacle::StaticClass();
}

void AGlitchZone::BeginPlay(){
	Super::BeginPlay();

	GetStaticMeshComponent()->OnComponentBeginOverlap.AddDynamic(this, &AGlitchZone::EnterGlitchZone);
	GetStaticMeshComponent()->OnComponentEndOverlap.AddDynamic(this, &AGlitchZone::ExitGlitchZone);

	GameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	TArray<AActor*> AudioManagerArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAudioManager::StaticClass(), AudioManagerArray);

	AudioManager = Cast<AAudioManager>(AudioManagerArray[0]);
}

void AGlitchZone::EnterGlitchZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		MainPlayer = Cast<AMainPlayer>(OtherActor);
		MainPlayer->SetInGlitchZone(true);

		MainPlayer->EnableGlitchEffect(true, GlitchFadeTime);

		AudioManager->SetParameter("Glitch_Zone", 1);

		GameMode->AddGlitch(GlitchGaugeValueToAddAtStart);
		GameMode->SetLevelState(ELevelState::Normal);

		GetWorld()->GetTimerManager().SetTimer(GlitchGaugeTimer, [&]() {
			GameMode->AddGlitch(GlitchGaugeValueToAddEveryTick);
		}, GlitchGaugeTick, true);
	}
}

void AGlitchZone::ExitGlitchZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		MainPlayer = Cast<AMainPlayer>(OtherActor);
		MainPlayer->SetInGlitchZone(false);

		MainPlayer->EnableGlitchEffect(false, GlitchFadeTime);

		AudioManager->SetParameter("Glitch_Zone", 0);

		GetWorld()->GetTimerManager().ClearTimer(GlitchGaugeTimer);
	}
}