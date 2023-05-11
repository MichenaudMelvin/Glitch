// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HearingTriggerComponent.h"
#include "Kismet/GameplayStatics.h"

UHearingTriggerComponent::UHearingTriggerComponent(){
	PrimaryComponentTick.bCanEverTick = false;
}

void UHearingTriggerComponent::BeginPlay(){
	Super::BeginPlay();

	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorList);

	for(int i = 0; i < ActorList.Num(); i++){
		UActorComponent* CurrentComp = ActorList[i]->GetComponentByClass(UHearingComponent::StaticClass());

		if(IsValid(CurrentComp)){
			HearingComponents.Add(Cast<UHearingComponent>(CurrentComp));
		}
	}
}

void UHearingTriggerComponent::MakeNoise(AActor* Investigator, const FVector NoiseLocation, const float Radius){
	for(int i = 0; i < HearingComponents.Num(); i++){
		if(IsValid(HearingComponents[i]) && Investigator->GetDistanceTo(HearingComponents[i]->GetOwner()) > Radius){
			continue;
		}

		HearingComponents[i]->RegisterSound(Investigator, NoiseLocation);
	}
}



