// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPatrol/Services/SetNextPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/AIPatrol/PatrolCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

USetNextPatrolPoint::USetNextPatrolPoint(){
	Interval = 1;
    RandomDeviation = 0;
	bCallTickOnSearchStart = true;
	
	PatrolPointKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(USetNextPatrolPoint, PatrolPointKey), AActor::StaticClass());
	CurrentPatrolPointIndex.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(USetNextPatrolPoint, PatrolPointKey));
}

void USetNextPatrolPoint::OnSearchStart(FBehaviorTreeSearchData& SearchData){
	Super::OnSearchStart(SearchData);
	PatrolPointList = Cast<APatrolCharacter>(SearchData.OwnerComp.GetAIOwner()->GetCharacter())->GetPatrolPointList();
}

void USetNextPatrolPoint::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds){
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();
	
	if(!PatrolPointKey.IsSet() || !CurrentPatrolPointIndex.IsSet()){
		PatrolPointKey.ResolveSelectedKey(*CurrentBlackboard->GetBlackboardAsset());
		CurrentPatrolPointIndex.ResolveSelectedKey(*CurrentBlackboard->GetBlackboardAsset());
	}

	int Index = CurrentBlackboard->GetValue<UBlackboardKeyType_Int>(CurrentPatrolPointIndex.GetSelectedKeyID());

	if(Index >= PatrolPointList.Num()){
		CurrentBlackboard->SetValue<UBlackboardKeyType_Int>(CurrentPatrolPointIndex.GetSelectedKeyID(), 0);
		Index = 0;
	}

	CurrentBlackboard->SetValue<UBlackboardKeyType_Object>(PatrolPointKey.GetSelectedKeyID(), PatrolPointList[Index]);
}
