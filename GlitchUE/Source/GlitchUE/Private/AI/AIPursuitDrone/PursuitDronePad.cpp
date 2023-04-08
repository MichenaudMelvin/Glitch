// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPursuitDrone/PursuitDronePad.h"
#include "AI/AIPursuitDrone/PursuitDrone.h"
#include "Components/CapsuleComponent.h"

APursuitDronePad::APursuitDronePad(){
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TechPadMesh(TEXT("/Game/Meshs/Drones/Pursuit/SK_Drones_Pursuit_Pad"));
	check(TechPadMesh.Succeeded());

	GetSkeletalMeshComponent()->SetSkeletalMesh(TechPadMesh.Object);
	GetSkeletalMeshComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetSkeletalMeshComponent()->SetCanEverAffectNavigation(false);

	MedMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MedMesh"));
	MedMesh->SetupAttachment(GetSkeletalMeshComponent());

	MedMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	MedMesh->SetCanEverAffectNavigation(false);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MedPadMesh(TEXT("/Game/Meshs/Drones/Pursuit/SK_Drone_Crystal_Pad"));
	check(MedPadMesh.Succeeded());

	MedMesh->SetSkeletalMesh(MedPadMesh.Object);

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Anim(TEXT("/Game/Meshs/Drones/Pursuit/AS_Drones_Pursuit_Pad"));
	check(Anim.Succeeded());

	PadAnim = Anim.Object;
}

void APursuitDronePad::PlayAnim(const bool bReverseAnim) const{
	GetSkeletalMeshComponent()->PlayAnimation(PadAnim, bReverseAnim);
	GetSkeletalMeshComponent()->SetPlayRate(bReverseAnim ? -1 : 1);

	if(bReverseAnim){
		FTimerHandle TimerHandle;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, GetSkeletalMeshComponent(), &USkeletalMeshComponent::Stop, PadAnim->GetMaxCurrentTime() - 0.1f, false);
	}
}

void APursuitDronePad::SetCurrentDrone(APursuitDrone* NewDrone){
	CurrentDrone = NewDrone;
}

void APursuitDronePad::OnConstruction(const FTransform& Transform){
	Super::OnConstruction(Transform);

	if(IsValid(CurrentDrone)){
		CurrentDrone->SetCurrentPad(this);

		FTransform TargetTransform = GetActorTransform();
		FVector TargetVector = TargetTransform.GetLocation();
		TargetVector.Z += CurrentDrone->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		TargetTransform.SetLocation(TargetVector);

		FRotator TargetRotator = TargetTransform.GetRotation().Rotator();

		TargetRotator.Yaw += 180;

		TargetTransform.SetRotation(TargetRotator.Quaternion());

		CurrentDrone->SetActorTransform(TargetTransform);
	}
}
