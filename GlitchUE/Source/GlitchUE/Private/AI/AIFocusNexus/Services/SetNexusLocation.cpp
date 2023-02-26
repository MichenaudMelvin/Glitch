// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIFocusNexus/Services/SetNexusLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/Nexus.h"

USetNexusLocation::USetNexusLocation(){
	Interval = 1;
	RandomDeviation = 0;
	
	NexusLocation.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(USetNexusLocation, NexusLocation));
}

void USetNexusLocation::InitializeFromAsset(UBehaviorTree& Asset){
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset)){
		NexusLocation.ResolveSelectedKey(*BBAsset);
	}
}

void USetNexusLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds){
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();
	
	if(!NexusLocation.IsSet()){
		NexusLocation.ResolveSelectedKey(*CurrentBlackboard->GetBlackboardAsset());
	}
	
	TArray<AActor*> NexusArray;
	UGameplayStatics::GetAllActorsOfClass(this, ANexus::StaticClass(), NexusArray);

	CurrentBlackboard->SetValue<UBlackboardKeyType_Vector>(NexusLocation.GetSelectedKeyID(), NexusArray[0]->GetActorLocation());
}
