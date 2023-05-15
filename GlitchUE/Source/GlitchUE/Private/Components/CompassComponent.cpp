// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CompassComponent.h"
#include "Components/CompassPivotIcon.h"
#include "Components/CompassIcon.h"

UCompassComponent::UCompassComponent(){
	PrimaryComponentTick.bCanEverTick = false;
}

void UCompassComponent::AddIconToOwnerList(UCompassIcon* IconToAdd){
	CompassIcons.Add(IconToAdd);

	UCompassPivotIcon* CurrentCompassPivotIcon = Cast<UCompassPivotIcon>(GetOwner()->AddComponentByClass(UCompassPivotIcon::StaticClass(), false, FTransform::Identity, false));
	CurrentCompassPivotIcon->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	CurrentCompassPivotIcon->InitPivotIcon(this, IconToAdd);

	CompassPivotIcons.Add(CurrentCompassPivotIcon);
}

void UCompassComponent::RemoveIconToOwnerList(UCompassIcon* IconToRemove){
	const int TargetIndex = CompassIcons.IndexOfByKey(IconToRemove);

	CompassPivotIcons[TargetIndex]->DestroyComponent();

	CompassPivotIcons.RemoveAt(TargetIndex);
	CompassIcons.RemoveAt(TargetIndex);
}

bool UCompassComponent::UsesZAxis() const{
	return bUsesZAxis;
}

