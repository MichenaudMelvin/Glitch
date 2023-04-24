// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralDecorators/CheckDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UCheckDistance::UCheckDistance(){
	Actor.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UCheckDistance, Actor), AActor::StaticClass());
	Distance.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UCheckDistance, Distance));
}

bool UCheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const{

	const UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();

	const AActor* TargetActor = Cast<AActor>(CurrentBlackboard->GetValue<UBlackboardKeyType_Object>(Actor.SelectedKeyName));

	const float DistanceToActor = OwnerComp.GetAIOwner()->GetPawn()->GetDistanceTo(TargetActor);

	const float DistanceCheck = CurrentBlackboard->GetValue<UBlackboardKeyType_Float>(Distance.SelectedKeyName);

	bool bResult = false;

	switch (CheckMethod) {
		case ECheckMethod::Equal:
			bResult = DistanceToActor == DistanceCheck;
			break;
		case ECheckMethod::NotEqual:
			bResult = DistanceToActor != DistanceCheck;
			break;
		case ECheckMethod::GreaterThanOrEqualTo:
			bResult = DistanceToActor >= DistanceCheck;
			break;
		case ECheckMethod::LessOrEqual:
			bResult = DistanceToActor <= DistanceCheck;
			break;
		case ECheckMethod::GreaterThan:
			bResult = DistanceToActor > DistanceCheck;
			break;
		case ECheckMethod::LessThan:
			bResult = DistanceToActor < DistanceCheck;
			break;
	}

	return bResult;
}
