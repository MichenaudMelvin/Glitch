// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPatrol/PatrolCharacter.h"

void APatrolCharacter::ReceiveGlitchUpgrade(){
	Super::ReceiveGlitchUpgrade();

	SightComp->SetWorldScale3D(GlitchScaleDetection);
}

void APatrolCharacter::ResetGlitchUpgrade(){
	Super::ResetGlitchUpgrade();

	SightComp->SetWorldScale3D(ScaleDetection);
}

TArray<APatrolPoint*> APatrolCharacter::GetPatrolPointList() const{
	return PatrolPointsList;
}
