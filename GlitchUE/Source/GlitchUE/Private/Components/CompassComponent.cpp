// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CompassComponent.h"
#include "Components/CompassPivotIcon.h"
#include "Components/CompassIcon.h"

UCompassComponent::UCompassComponent(){
	PrimaryComponentTick.bCanEverTick = false;
}

void UCompassComponent::DestroyComponent(bool bPromoteChildren){
	Super::DestroyComponent(bPromoteChildren);

	for(int i = 0; i < CompassPivotIcons.Num(); i++){
		if(!CompassPivotIcons[i]){
			continue;
		}

		CompassPivotIcons[i]->DestroyComponent();
	}
}

void UCompassComponent::AddIconToOwnerList(UCompassIcon* IconToAdd){
	CompassIcons.Add(IconToAdd);

	FTransform TargetTransform = FTransform::Identity;
	TargetTransform.SetLocation(OffsetIcon);

	UCompassPivotIcon* CurrentCompassPivotIcon = Cast<UCompassPivotIcon>(GetOwner()->AddComponentByClass(UCompassPivotIcon::StaticClass(), false, TargetTransform, false));
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

float UCompassComponent::GetCompassRadius() const{
	return CompassRadius;
}

void UCompassComponent::SetCompassOffset(const FVector NewOffset){
	OffsetIcon = NewOffset;
}
