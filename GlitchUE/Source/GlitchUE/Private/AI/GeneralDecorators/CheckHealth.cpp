// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralDecorators/CheckHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Components/HealthComponent.h"

UCheckHealth::UCheckHealth(){
	DestructibleObject.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UCheckHealth, DestructibleObject), AActor::StaticClass());
}

bool UCheckHealth::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const{

	const UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();

	const AActor* TargetObject = Cast<AActor>(CurrentBlackboard->GetValue<UBlackboardKeyType_Object>(DestructibleObject.SelectedKeyName));

	if(!IsValid(TargetObject)){
		return false;
	}

	const UHealthComponent* TargetHealthComp = Cast<UHealthComponent>(TargetObject->GetComponentByClass(UHealthComponent::StaticClass()));

	bool bResult = false;

	switch (CheckMethod) {
	case ECheckMethod::Equal:
		bResult = TargetHealthComp->GetCurrentHealth() == CheckValue;
		break;
	case ECheckMethod::NotEqual:
		bResult = TargetHealthComp->GetCurrentHealth() != CheckValue;
		break;
	case ECheckMethod::GreaterThanOrEqualTo:
		bResult = TargetHealthComp->GetCurrentHealth() >= CheckValue;
		break;
	case ECheckMethod::LessOrEqual:
		bResult = TargetHealthComp->GetCurrentHealth() <= CheckValue;
		break;
	case ECheckMethod::GreaterThan:
		bResult = TargetHealthComp->GetCurrentHealth() > CheckValue;
		break;
	case ECheckMethod::LessThan:
		bResult = TargetHealthComp->GetCurrentHealth() < CheckValue;
		break;
	}

	if(bInvertResult)
	{
		return !bResult;
	}

	return bResult;
}
