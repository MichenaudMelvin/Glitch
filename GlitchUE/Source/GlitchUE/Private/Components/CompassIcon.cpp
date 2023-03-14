// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CompassIcon.h"

UCompassIcon::UCompassIcon(){
	PrimaryComponentTick.bCanEverTick = false;
	
}


void UCompassIcon::BeginPlay(){
	Super::BeginPlay();

	OwnerLocation = GetOwner()->GetActorLocation();
}

UPaperSprite* UCompassIcon::GetOwnerSprite(){
	return OwnerSprite;
}

FVector UCompassIcon::GetOwnerLocation(){
	return OwnerLocation;
}
