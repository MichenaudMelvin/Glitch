// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICamera/CameraPawn.h"

ACameraPawn::ACameraPawn(){
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

}

void ACameraPawn::BeginPlay(){
	Super::BeginPlay();
	
}
