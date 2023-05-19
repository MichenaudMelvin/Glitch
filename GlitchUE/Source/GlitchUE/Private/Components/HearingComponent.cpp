// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HearingComponent.h"
#include "AI/MainAICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Physic/CustomPhysicalMat.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

UHearingComponent::UHearingComponent(){
	PrimaryComponentTick.bCanEverTick = false;

	ClassToIgnore.Add(AMainAICharacter::StaticClass());
}

void UHearingComponent::BeginPlay(){
	Super::BeginPlay();

	BlackboardOwner = Cast<APawn>(GetOwner())->Controller->FindComponentByClass<UBlackboardComponent>();

	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorList);

	for(int i = 0; i < ActorList.Num(); i++){
		UActorComponent* CurrentComp = ActorList[i]->GetComponentByClass(UHearingTriggerComponent::StaticClass());

		if(IsValid(CurrentComp)){
			Cast<UHearingTriggerComponent>(CurrentComp)->AddHearingComp(this);
		}
	}
}

void UHearingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason){
	Super::EndPlay(EndPlayReason);

	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorList);

	for(int i = 0; i < ActorList.Num(); i++){
		UActorComponent* CurrentComp = ActorList[i]->GetComponentByClass(UHearingTriggerComponent::StaticClass());

		if(IsValid(CurrentComp)){
			Cast<UHearingTriggerComponent>(CurrentComp)->RemoveHearingComp(this);
		}
	}
}

void UHearingComponent::RegisterSound(AActor* Investigator, const FVector SoundLocation){
	CurrentSoundLocation = SoundLocation;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	ActorsToIgnore.Add(Investigator);

	TArray<FHitResult> HitResults;

	UKismetSystemLibrary::LineTraceMulti(GetWorld(), GetOwner()->GetActorLocation(), SoundLocation, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel6), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	if(HitResults.Num() == 0){
		HearSound();
		return;
	}

	float CurrentSoundTolerance = 0;

	for(int i = 0; i < HitResults.Num(); i++){
		if(HitResults[i].PhysMaterial.Get()->IsA(UCustomPhysicalMat::StaticClass())){
			const UCustomPhysicalMat* CurrentPhysicMat = Cast<UCustomPhysicalMat>(HitResults[i].PhysMaterial.Get());

			CurrentSoundTolerance += CurrentPhysicMat->GetSoundResistance();

			if(CurrentSoundTolerance >= MaxSoundTolerance){
				return;
			}
		} else{
			return;
		}
	}

	HearSound();
}

void UHearingComponent::HearSound() const{
	if(BlackboardOwner->GetValueAsBool("HearSound")){
		BlackboardOwner->SetValueAsBool("IsMovingToHearingLocation", false);
	} else{
		BlackboardOwner->SetValueAsBool("HearSound", true);
	}

	BlackboardOwner->SetValueAsVector("HearingLocation", CurrentSoundLocation);
}

bool UHearingComponent::IsClassValid(const AActor* ActorToCheck) const{
	for(int i = 0; i < ClassToIgnore.Num(); i++){
		if(!IsValid(ClassToIgnore[i])){
			continue;
		}

		if(ActorToCheck->IsA(ClassToIgnore[i])){
			return false;
		}
	}

	return true;
}