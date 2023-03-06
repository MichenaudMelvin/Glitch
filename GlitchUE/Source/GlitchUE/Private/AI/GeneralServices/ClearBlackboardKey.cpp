// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralServices/ClearBlackboardKey.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UClearBlackboardKey::UClearBlackboardKey(){
	Interval = 1;
	RandomDeviation = 0;
	
}

void UClearBlackboardKey::InitializeFromAsset(UBehaviorTree& Asset){
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset)){
		Key.ResolveSelectedKey(*BBAsset);
	}
}

void UClearBlackboardKey::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds){
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();

	CurrentBlackboard->ClearValue(Key.SelectedKeyName);
}
