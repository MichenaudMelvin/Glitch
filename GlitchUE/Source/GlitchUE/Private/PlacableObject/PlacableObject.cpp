// Fill out your copyright notice in the Description page of Project Settings.

#include "PlacableObject/PlacableObject.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UPlacableObject::UPlacableObject()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPlacableObject::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetTargetLocation();
	
}

FVector UPlacableObject::GetOriginalLocation() {
	return OriginalLocation;
}


bool UPlacableObject::PreviewObject(){
	FHitResult Hit;
	FVector Point = GetOwner()->GetActorLocation();
	FCollisionQueryParams QueryParams;
	FCollisionResponseParams Responseparam;
	return !GetWorld()->LineTraceSingleByChannel(Hit, Point, Point, ECollisionChannel::ECC_Visibility, QueryParams, Responseparam);
}


void UPlacableObject::PlaceObject(FVector NewLocation){
	GetOwner()->SetActorLocation(NewLocation);
}

void UPlacableObject::ResetObject(){
	GetOwner()->SetActorLocation(OriginalLocation);
}


// Called every frame
void UPlacableObject::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}