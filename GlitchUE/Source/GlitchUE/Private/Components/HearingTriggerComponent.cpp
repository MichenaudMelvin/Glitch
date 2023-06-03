// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HearingTriggerComponent.h"

#include "PopcornFXAttributeFunctions.h"
#include "Kismet/GameplayStatics.h"

UHearingTriggerComponent::UHearingTriggerComponent(){
	PrimaryComponentTick.bCanEverTick = false;
}

void UHearingTriggerComponent::MakeNoise(AActor* Investigator, const FVector NoiseLocation, const float Radius, UPopcornFXEmitterComponent* SoundFX){
	if(IsValid(SoundFX)){
		const int FXRadiusIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(SoundFX, "SizeMul");
		UPopcornFXAttributeFunctions::SetAttributeAsFloat(SoundFX, FXRadiusIndex, Radius, true);
		SoundFX->StartEmitter();
	}

	for(int i = 0; i < HearingComponents.Num(); i++){
		if(!IsValid(HearingComponents[i])){
			continue;
		}

		if(Investigator->GetDistanceTo(HearingComponents[i]->GetOwner()) > Radius){
			continue;
		}

		HearingComponents[i]->RegisterSound(Investigator, NoiseLocation);
	}
}

void UHearingTriggerComponent::AddHearingComp(UHearingComponent* CompToAdd){
	HearingComponents.Add(CompToAdd);
}

void UHearingTriggerComponent::RemoveHearingComp(UHearingComponent* CompToRemove){
	HearingComponents.Remove(CompToRemove);
}



