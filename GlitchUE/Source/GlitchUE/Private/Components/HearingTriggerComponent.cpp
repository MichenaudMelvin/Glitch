// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HearingTriggerComponent.h"
#include "Kismet/GameplayStatics.h"

UHearingTriggerComponent::UHearingTriggerComponent(){
	PrimaryComponentTick.bCanEverTick = false;
}

void UHearingTriggerComponent::MakeNoise(AActor* Investigator, const FVector NoiseLocation, const float Radius){
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



