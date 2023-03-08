// Fill out your copyright notice in the Description page of Project Settings.


#include "GlitchZones/GlitchZone.h"
#include "Components/ShapeComponent.h"
#include "Player/MainPlayer.h"

AGlitchZone::AGlitchZone(){
	GetSpriteComponent()->bHiddenInGame = true;
}

void AGlitchZone::BeginPlay(){
	Super::BeginPlay();

	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AGlitchZone::EnterGlitchZone);
	GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &AGlitchZone::ExitGlitchZone);
}

void AGlitchZone::EnterGlitchZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		Cast<AMainPlayer>(OtherActor)->SetInGlitchZone(true);
	}
}

void AGlitchZone::ExitGlitchZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		Cast<AMainPlayer>(OtherActor)->SetInGlitchZone(false);
	}
}
