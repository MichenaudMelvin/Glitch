// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralServices/SetBoolKey.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

USetBoolKey::USetBoolKey(){
	Interval = 1;
	RandomDeviation = 0;
	
	BoolKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(USetBoolKey, BoolKey));
}

void USetBoolKey::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds){
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();
	
	if(!BoolKey.IsSet()){
		BoolKey.ResolveSelectedKey(*CurrentBlackboard->GetBlackboardAsset());
	}
	
	CurrentBlackboard->SetValue<UBlackboardKeyType_Bool>(BoolKey.GetSelectedKeyID(), bBoolValue);
}
