// Fill out your copyright notice in the Description page of Project Settings.
	
#include "AI/GeneralTaks/BTTask_LookAtActor.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_LookAtActor::UBTTask_LookAtActor(){
	NodeName = "Look at Target";
	bNotifyTick = true;

	Target.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_LookAtActor, Target), AActor::StaticClass());
	Target.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_LookAtActor, Target));
}

EBTNodeResult::Type UBTTask_LookAtActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* AIPawn = AIController->GetPawn();
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	
	FVector TargetLocation;

	if (Target.SelectedKeyType == UBlackboardKeyType_Object::StaticClass()) {
		UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(Target.GetSelectedKeyID());
		TargetLocation = Cast<AActor>(KeyValue)->GetActorLocation();
	}
	
	else if (Target.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()) {
		TargetLocation = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(Target.GetSelectedKeyID());
	}

	AIPawn->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(AIPawn->GetActorLocation(), TargetLocation));

	return EBTNodeResult::Succeeded;
}