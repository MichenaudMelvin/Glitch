// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TargetCameraLocation.h"

ATargetCameraLocation::ATargetCameraLocation(){
	PrimaryActorTick.bCanEverTick = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Root Component"));
	SetRootComponent(Camera);

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	ArrowComponent->SetupAttachment(RootComponent);
#endif
}

void ATargetCameraLocation::OnConstruction(const FTransform& Transform){
	Super::OnConstruction(Transform);

	Camera->FieldOfView = CameraFOV;
}
