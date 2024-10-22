// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPatrol/Services/SetNextPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/AIPatrol/PatrolCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "AI/AIPatrol/PatrolPoint.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

USetNextPatrolPoint::USetNextPatrolPoint(){
	Interval = 1;
	RandomDeviation = 0;

	PatrolPointKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(USetNextPatrolPoint, PatrolPointKey), AActor::StaticClass());
	CurrentPatrolPointIndex.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(USetNextPatrolPoint, PatrolPointKey));
}

void USetNextPatrolPoint::InitializeFromAsset(UBehaviorTree& Asset){
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset)){
		PatrolPointKey.ResolveSelectedKey(*BBAsset);
		CurrentPatrolPointIndex.ResolveSelectedKey(*BBAsset);
	}
}

void USetNextPatrolPoint::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds){
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();

	int Index = CurrentBlackboard->GetValue<UBlackboardKeyType_Int>(CurrentPatrolPointIndex.GetSelectedKeyID());

	TArray<APatrolPoint*> PatrolPointList = Cast<APatrolCharacter>(OwnerComp.GetAIOwner()->GetCharacter())->GetPatrolPointList();

	if(Index >= PatrolPointList.Num()){
		CurrentBlackboard->SetValue<UBlackboardKeyType_Int>(CurrentPatrolPointIndex.GetSelectedKeyID(), 0);
		Index = 0;
	}

	CurrentBlackboard->SetValue<UBlackboardKeyType_Object>(PatrolPointKey.GetSelectedKeyID(), PatrolPointList[Index]);
}
