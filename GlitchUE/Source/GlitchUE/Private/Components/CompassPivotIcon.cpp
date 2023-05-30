// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CompassPivotIcon.h"
#include "Components/CompassComponent.h"
#include "Kismet/KismetMathLibrary.h"

UCompassPivotIcon::UCompassPivotIcon(){
	PrimaryComponentTick.bCanEverTick = true;

	ChildMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ChildMesh->SetupAttachment(this);

	ChildMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ChildMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void UCompassPivotIcon::OnComponentDestroyed(bool bDestroyingHierarchy){
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	ChildMesh->DestroyComponent();
}

void UCompassPivotIcon::SelectRotation(){
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), TargetToLookAt->GetActorLocation());

	if(!Compass->UsesZAxis()){
		TargetRotation.Pitch = 0;
	}

	SetWorldRotation(TargetRotation);
}

void UCompassPivotIcon::SelectScale() const{
	if(!CurrentCompassIcon->UseDynamicScale()){
		return;
	}

	const float Alpha = UKismetMathLibrary::NormalizeToRange(CurrentCompassIcon->GetDistanceFromTarget(), Compass->GetCompassRadius(), CurrentCompassIcon->GetDrawDistance());

	const FVector TargetScale = UKismetMathLibrary::VLerp(CurrentCompassIcon->GetTargetScale(), FVector::ZeroVector, Alpha);
	ChildMesh->SetWorldScale3D(TargetScale);
}

void UCompassPivotIcon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!CurrentCompassIcon->ShouldUseTick()){
		return;
	}

	if(CurrentCompassIcon->CanBeDrawn()){
		ChildMesh->SetVisibility(true);
		SelectRotation();
		SelectScale();
	} else if(ChildMesh->IsVisible()){
		ChildMesh->SetVisibility(false);
	}
}

void UCompassPivotIcon::InitPivotIcon(UCompassComponent* CompassComp, UCompassIcon* TargetCompassIcon){
	Compass = CompassComp;

	CurrentCompassIcon = TargetCompassIcon;

	TargetToLookAt = CurrentCompassIcon->GetOwner();
	ChildMesh->SetStaticMesh(CurrentCompassIcon->GetOwnerMesh());

	FTransform TargetTransform = FTransform::Identity;
	TargetTransform.SetScale3D(CurrentCompassIcon->GetTargetScale());
	TargetTransform.SetLocation(FVector(Compass->GetCompassRadius(), 0, 0));

	ChildMesh->SetRelativeTransform(TargetTransform);

	if(!CurrentCompassIcon->ShouldUseTick() && CurrentCompassIcon->CanBeDrawn()){
		SelectRotation();
	}
}

