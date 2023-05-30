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

	if(!bSetupAuto){
		return;
	}

	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TargetCompassOwnerClass, ActorList);

	#if WITH_EDITOR
		// for simulation mode
		if(ActorList.Num() == 0){
			return;
		}
	#endif

	UCompassComponent* CompassComp = Cast<UCompassComponent>(ActorList[0]->GetComponentByClass(UCompassComponent::StaticClass()));

	if(!IsValid(CompassComp)){
		UE_LOG(LogActorComponent, Warning, TEXT("Compass owner is not valid for %s"), *this->GetName());
		return;
	}

	SetCompassOwner(CompassComp);
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

	return GetDistanceFromTarget() <= DrawDistance;
}

void UCompassIcon::SetAllowDraw(const bool bAllow){
	bAllowDraw = bAllow;
}

FVector UCompassIcon::GetTargetScale() const{
	return FVector(TargetScale, TargetScale, TargetScale);
}

void UCompassIcon::SetDrawDistance(const float NewDistance){
	DrawDistance = NewDistance;
}

float UCompassIcon::GetDrawDistance() const{
	return DrawDistance;
}

void UCompassIcon::SetOwnerClass(const TSubclassOf<AActor> NewOwnerClass){
	TargetCompassOwnerClass = NewOwnerClass;
}

void UCompassIcon::SetupAuto(const bool bAuto){
	bSetupAuto = bAuto;
}

void UCompassIcon::SetShouldUseTick(const bool bShouldUseTick){
	bTickDraw = bShouldUseTick;
}

bool UCompassIcon::ShouldUseTick() const{
	return bTickDraw;
}

void UCompassIcon::SetCompassOwner(UCompassComponent* NewOwner){
	CompassOwner = NewOwner;
	CompassOwner->AddIconToOwnerList(this);
}

float UCompassIcon::GetDistanceFromTarget() const{
	return CompassOwner->GetOwner()->GetDistanceTo(GetOwner());
}

bool UCompassIcon::UseDynamicScale() const{
	return bUseDynamicScale;
}
