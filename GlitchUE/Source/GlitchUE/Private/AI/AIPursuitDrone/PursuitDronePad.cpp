// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPursuitDrone/PursuitDronePad.h"

APursuitDronePad::APursuitDronePad(){
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PadMesh(TEXT("/Game/Meshs/Drones/SK_Drones_Pursuit_Pad"));
	check(PadMesh.Succeeded());

	GetSkeletalMeshComponent()->SetSkeletalMesh(PadMesh.Object);
	GetSkeletalMeshComponent()->SetRelativeRotation(FRotator(0, 180, 0));

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Anim(TEXT("/Game/Meshs/Drones/AS_Drones_Pursuit_Pad"));
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
