// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPatrol/PatrolCharacter.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/Selection.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"

APatrolCharacter::APatrolCharacter(){
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMesh(TEXT("/Game/Meshs/Drones/Seeker/SK_Seeker"));
	check(SkelMesh.Succeeded());

	GetMesh()->SetSkeletalMesh(SkelMesh.Object, false);

#if WITH_EDITORONLY_DATA
	USelection::SelectObjectEvent.AddUObject(this, &APatrolCharacter::OnObjectSelected);
	USelection::SelectionChangedEvent.AddUObject(this, &APatrolCharacter::OnObjectSelected);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> EditorMesh(TEXT("/Game/Meshs/EditorOnly/SM_DronePath"));
	check(EditorMesh.Succeeded());

	CubeMesh = EditorMesh.Object;
#endif
}

TArray<APatrolPoint*> APatrolCharacter::GetPatrolPointList() const{
	return PatrolPointsList;
}

#if WITH_EDITORONLY_DATA
void APatrolCharacter::PreEditChange(FProperty* PropertyAboutToChange){
	Super::PreEditChange(PropertyAboutToChange);

	OutlineLinkedObjects(false);
}

void APatrolCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent){
	Super::PostEditChangeProperty(PropertyChangedEvent);

	OutlineLinkedObjects(true);
}

void APatrolCharacter::OnObjectSelected(UObject* Object){
	if (Object == this) {

		OutlineLinkedObjects(true);

	} else if (!IsSelected()){

		OutlineLinkedObjects(false);
	}
}

void APatrolCharacter::OutlineLinkedObjects(const bool bOutline){
	for(int i = 0; i < PatrolPointsList.Num(); i++){
		if(IsValid(PatrolPointsList[i])){
			UUsefullFunctions::OutlineComponent(bOutline, Cast<UPrimitiveComponent>(PatrolPointsList[i]->GetRootComponent()));
		}
	}

	if(bOutline){
		for(int i = 0; i < PatrolPointsList.Num(); i++){
			if(!IsValid(PatrolPointsList[i])){
				continue;
			}

			FActorSpawnParameters ActorSpawnParameters;
			ASplineMeshActor* CurrentSplineActor = GetWorld()->SpawnActor<ASplineMeshActor>(ASplineMeshActor::StaticClass(), PatrolPointsList[i]->GetActorLocation(), FRotator::ZeroRotator, ActorSpawnParameters);

			CurrentSplineActor->GetSplineMeshComponent()->SetStaticMesh(CubeMesh);

			CurrentSplineActor->GetSplineMeshComponent()->SetStartPosition(FVector::ZeroVector);

			const int TargetIndex = PatrolPointsList.IsValidIndex(i + 1) && IsValid(PatrolPointsList[i + 1]) ? i + 1 : 0;

			if(!IsValid(PatrolPointsList[TargetIndex])){
				continue;
			}

			const FVector TargetLocation = PatrolPointsList[TargetIndex]->GetActorLocation() - PatrolPointsList[i]->GetActorLocation();
			CurrentSplineActor->GetSplineMeshComponent()->SetEndPosition(TargetLocation);

			SplineList.Add(CurrentSplineActor);
		}
	} else{
		for(int i = 0; i < SplineList.Num(); i++){
			SplineList[i]->Destroy();
		}

		SplineList.Empty();
	}
}

void APatrolCharacter::PreSave(const ITargetPlatform* TargetPlatform){
	Super::PreSave(TargetPlatform);

	OutlineLinkedObjects(false);
}
#endif
