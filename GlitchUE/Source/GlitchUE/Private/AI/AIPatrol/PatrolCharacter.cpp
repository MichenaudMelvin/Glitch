// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPatrol/PatrolCharacter.h"

#include "Engine/Selection.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"

APatrolCharacter::APatrolCharacter(){
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMesh(TEXT("/Game/Meshs/Drones/Seeker/SK_Seeker"));
	check(SkelMesh.Succeeded());

	GetMesh()->SetSkeletalMesh(SkelMesh.Object, false);

	#if WITH_EDITORONLY_DATA
		USelection::SelectObjectEvent.AddUObject(this, &APatrolCharacter::OnObjectSelected);
	#endif
}

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

#if WITH_EDITORONLY_DATA
void APatrolCharacter::OnObjectSelected(UObject* Object){
	if (Object == this) {

		for(int i = 0; i < PatrolPointsList.Num(); i++){
			if(IsValid(PatrolPointsList[i])){
				UUsefullFunctions::OutlineComponent(true, Cast<UPrimitiveComponent>(PatrolPointsList[i]->GetRootComponent()));
			}
		}

	} else if (!IsSelected()){

		for(int i = 0; i < PatrolPointsList.Num(); i++){
			if(IsValid(PatrolPointsList[i])){
				UUsefullFunctions::OutlineComponent(false, Cast<UPrimitiveComponent>(PatrolPointsList[i]->GetRootComponent()));
			}
		}
	}
}
#endif