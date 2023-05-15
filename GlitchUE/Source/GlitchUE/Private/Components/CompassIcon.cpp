// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CompassIcon.h"
#include "Components/CompassComponent.h"
#include "Kismet/GameplayStatics.h"

UCompassIcon::UCompassIcon(){
	PrimaryComponentTick.bCanEverTick = false;
}

void UCompassIcon::BeginPlay(){
	Super::BeginPlay();

	OwnerLocation = GetOwner()->GetActorLocation();

	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TargetCompassOwnerClass, ActorList);

	#if WITH_EDITOR
		// for simulation mode
		if(ActorList.Num() == 0){
			return;
		}
	#endif

	CompassOwner = Cast<UCompassComponent>(ActorList[0]->GetComponentByClass(UCompassComponent::StaticClass()));
	CompassOwner->AddIconToOwnerList(this);
}

void UCompassIcon::OnComponentDestroyed(bool bDestroyingHierarchy){
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	if(IsValid(CompassOwner)){
		CompassOwner->RemoveIconToOwnerList(this);
	}
}

UStaticMesh* UCompassIcon::GetOwnerMesh() const{
	return OwnerMesh;
}

FVector UCompassIcon::GetOwnerLocation() const{
	return OwnerLocation;
}

bool UCompassIcon::CanBeDrawn() const{
	if(!bAllowDraw){
		return false;
	}

	if(DrawDistance <= 0){
		return true;
	}

	return CompassOwner->GetOwner()->GetDistanceTo(GetOwner()) <= DrawDistance;
}

void UCompassIcon::SetAllowDraw(const bool bAllow){
	bAllowDraw = bAllow;
}

FVector UCompassIcon::GetTargetScale() const{
	return FVector(TargetScale, TargetScale, TargetScale);
}
