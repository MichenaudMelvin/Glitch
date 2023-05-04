// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/GeneralServices/SetPlayerPosition.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

USetPlayerPosition::USetPlayerPosition(){
	Interval = 1;
	RandomDeviation = 0;

	Player.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(USetPlayerPosition, Player), AActor::StaticClass());
	PlayerPosition.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(USetPlayerPosition, PlayerPosition));
}

void USetPlayerPosition::InitializeFromAsset(UBehaviorTree& Asset){
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset)){
		Player.ResolveSelectedKey(*BBAsset);
		PlayerPosition.ResolveSelectedKey(*BBAsset);
	}
}

void USetPlayerPosition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds){
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();

	const AActor* PlayerReference = Cast<AActor>(CurrentBlackboard->GetValue<UBlackboardKeyType_Object>(Player.GetSelectedKeyID()));

	CurrentBlackboard->SetValue<UBlackboardKeyType_Vector>(PlayerPosition.GetSelectedKeyID(), PlayerReference->GetActorLocation());
}
