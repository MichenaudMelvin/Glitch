// Fill out your copyright notice in the Description page of Project Settings.


#include "GlitchZones/GlitchZone.h"
#include "Audio/AudioManager.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayer.h"

AGlitchZone::AGlitchZone(){
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Engine/BasicShapes/Cube1"));
	check(Mesh.Succeeded());

	GetStaticMeshComponent()->SetStaticMesh(Mesh.Object);
	GetStaticMeshComponent()->SetWorldScale3D(FVector(2, 2, 2));
}

void AGlitchZone::BeginPlay(){
	Super::BeginPlay();

	TArray<AActor*> AudioManagerArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAudioManager::StaticClass(), AudioManagerArray);

	AudioManager = Cast<AAudioManager>(AudioManagerArray[0]);
}

void AGlitchZone::OnPlayerEnterZone(){
	Super::OnPlayerEnterZone();

	MainPlayer->EnableGlitchEffect(true, PostProcessFadeTime);

	AudioManager->SetParameter("Glitch_Zone", 1);

	GameMode->AddGlitch(GlitchGaugeValueToAddAtStart);
	GameMode->SetLevelState(ELevelState::Normal);

	GetWorld()->GetTimerManager().SetTimer(ZoneTimer, [&]() {
		GameMode->AddGlitch(GlitchGaugeValueToAddEveryTick);
	}, GlitchGaugeTick, true);
}

void AGlitchZone::OnPlayerExitZone(){
	Super::OnPlayerExitZone();

	MainPlayer->EnableGlitchEffect(false, PostProcessFadeTime);

	AudioManager->SetParameter("Glitch_Zone", 0);
}