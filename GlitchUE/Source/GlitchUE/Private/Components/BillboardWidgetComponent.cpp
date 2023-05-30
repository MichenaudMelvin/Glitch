// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BillboardWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UBillboardWidgetComponent::BeginPlay(){
	Super::BeginPlay();

	CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
}

void UBillboardWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SelectRotation();
}

void UBillboardWidgetComponent::SelectRotation(){
	if(!IsValid(CameraManager) || !bEnableBillboardBehavior){
		return;
	}

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CameraManager->GetCameraLocation(), GetOwner()->GetActorLocation());
	TargetRotation.Pitch *= -1;
	TargetRotation.Yaw += 180;
	SetWorldRotation(TargetRotation);
}
