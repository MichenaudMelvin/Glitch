// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CompassPivotIcon.h"
#include "Components/CompassComponent.h"
#include "Kismet/KismetMathLibrary.h"

UCompassPivotIcon::UCompassPivotIcon(){
	PrimaryComponentTick.bCanEverTick = true;

	ChildMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ChildMesh->SetupAttachment(this);

	FTransform TargetTransform = FTransform::Identity;
	TargetTransform.SetLocation(FVector(SpriteOffset, 0, 0));

	ChildMesh->SetRelativeTransform(TargetTransform);

	ChildMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ChildMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void UCompassPivotIcon::OnComponentDestroyed(bool bDestroyingHierarchy){
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	ChildMesh->DestroyComponent();
}

void UCompassPivotIcon::SelectorRotation(){
	FRotator TargetLocation = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), TargetToLookAt->GetActorLocation());

	if(!Compass->UsesZAxis()){
		TargetLocation.Pitch = 0;
	}

	SetWorldRotation(TargetLocation);
}

void UCompassPivotIcon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(CurrentCompassIcon->CanBeDrawn()){
		ChildMesh->SetVisibility(true);
		SelectorRotation();
	} else if(ChildMesh->IsVisible()){
		ChildMesh->SetVisibility(false);
	}
}

void UCompassPivotIcon::InitPivotIcon(UCompassComponent* CompassComp, UCompassIcon* TargetCompassIcon){
	Compass = CompassComp;

	CurrentCompassIcon = TargetCompassIcon;
	ChildMesh->SetRelativeScale3D(CurrentCompassIcon->GetTargetScale());

	TargetToLookAt = CurrentCompassIcon->GetOwner();
	ChildMesh->SetStaticMesh(CurrentCompassIcon->GetOwnerMesh());
}

