// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralDecorators/IsPlayerInSight.h"
#include "AIController.h"
#include "Components/SightComponent.h"

bool UIsPlayerInSight::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const{
	const USightComponent* SightComp = Cast<USightComponent>(OwnerComp.GetAIOwner()->GetPawn()->GetComponentByClass(USightComponent::StaticClass()));

	if(bInvertResult){
		return !SightComp->IsPlayerInSight();
	}

	return SightComp->IsPlayerInSight();
}
