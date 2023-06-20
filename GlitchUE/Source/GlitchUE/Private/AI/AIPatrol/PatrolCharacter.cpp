// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPatrol/PatrolCharacter.h"
#include "PopcornFXAttributeFunctions.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/Selection.h"
#include "AI/MainAIData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Helpers/FunctionsLibrary/UsefulFunctions.h"
#include "Kismet/GameplayStatics.h"

APatrolCharacter::APatrolCharacter(){
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMesh(TEXT("/Game/Meshs/Drones/Seeker/SK_Seeker"));
	check(SkelMesh.Succeeded());

	GetMesh()->SetSkeletalMesh(SkelMesh.Object, false);

	HearingComponent = CreateDefaultSubobject<UHearingComponent>(TEXT("Hearing Comp"));

	SightWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Sight Widget"));
	SightWidget->SetupAttachment(GetMesh());

	SightWidget->SetRelativeLocation(FVector(0, 0, 200));
	SightWidget->SetRelativeRotation(FRotator(0, 90, 0));

	SightWidget->SetWorldScale3D(FVector(0.25,0.05,0.05));
	SightWidget->SetDrawSize(FVector2D(1920, 1080));

	SightWidget->SetGenerateOverlapEvents(false);

	DetectionFX = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("Detection FX"));
	DetectionFX->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> DetectionEffect(TEXT("/Game/VFX/Particles/FX_Enemies/Pk_DroneSpotted"));
	check(DetectionEffect.Succeeded());

	DetectionFX->SetRelativeRotation(FRotator(0, -90, 0));
	DetectionFX->bPlayOnLoad = false;

	DetectionFX->SetEffect(DetectionEffect.Object);

#if WITH_EDITORONLY_DATA
	USelection::SelectObjectEvent.AddUObject(this, &APatrolCharacter::OnObjectSelected);
	USelection::SelectionChangedEvent.AddUObject(this, &APatrolCharacter::OnObjectSelected);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> EditorMesh(TEXT("/Game/Meshs/EditorOnly/SM_DronePath"));
	check(EditorMesh.Succeeded());

	CubeMesh = EditorMesh.Object;
#endif
}

void APatrolCharacter::BeginPlay(){
	Super::BeginPlay();

	Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->OnSwitchLevelState.AddDynamic(this, &APatrolCharacter::OnSwitchLevelState);
}

void APatrolCharacter::Destroyed(){
	Super::Destroyed();

	#if WITH_EDITORONLY_DATA
		OutlineLinkedObjects(false);
	#endif
}

void APatrolCharacter::OnSwitchLevelState(ELevelState NewLevelState){
	switch (NewLevelState){
		case ELevelState::Normal:
			GetCharacterMovement()->MaxWalkSpeed = CurrentData->Speed;
			break;
		case ELevelState::Alerted:
			GetCharacterMovement()->MaxWalkSpeed = CurrentData->GlitchSpeed;
			break;
	}
}

TArray<APatrolPoint*> APatrolCharacter::GetPatrolPointList() const{
	return PatrolPointsList;
}

void APatrolCharacter::SetDetectionValue(const float DetectionValue) const{
	if(Blackboard->GetValueAsBool("ReceiveAlert")){
		return;
	}

	const int TargetIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(DetectionFX, "GaugeCursor");

	float Value;
	UPopcornFXAttributeFunctions::GetAttributeAsFloat(DetectionFX, TargetIndex, Value, false);

	if(Value >= 1){
		DetectionFX->StopEmitter(true);
		DetectionFX->StartEmitter();
	}

	UPopcornFXAttributeFunctions::SetAttributeAsFloat(DetectionFX, TargetIndex, FMath::Abs(DetectionValue - 1), false);
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

void APatrolCharacter::PostEditUndo(){
	Super::PostEditUndo();

	OutlineLinkedObjects(false);

	TArray<AActor*> SplineActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASplineMeshActor::StaticClass(), SplineActorList);

	for(int i = 0; i < SplineActorList.Num(); i++){
		SplineActorList[i]->Destroy();
	}
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
			UUsefulFunctions::OutlineComponent(bOutline, Cast<UPrimitiveComponent>(PatrolPointsList[i]->GetRootComponent()));
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
