// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralDecorators/IsSomethingInSight.h"
#include "AIController.h"
#include "Components/SightComponent.h"

bool UIsSomethingInSight::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const{
	const USightComponent* SightComp = Cast<USightComponent>(OwnerComp.GetAIOwner()->GetPawn()->GetComponentByClass(USightComponent::StaticClass()));

	if(bInvertResult){
		return !SightComp->IsSomethingInSight();
	}

	return SightComp->IsSomethingInSight();
}
