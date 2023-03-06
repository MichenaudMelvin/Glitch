// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPursuitDrone/PursuitDrone.h"
#include "Kismet/GameplayStatics.h"

APursuitDrone::APursuitDrone(){
	PrimaryActorTick.bCanEverTick = false;

	// BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	//
	// SetRootComponent(BoxComp);
	//
	// AIMesh->SetupAttachment(RootComponent);
	//
	// MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
}

void APursuitDrone::BeginPlay(){
	Super::BeginPlay();

}

