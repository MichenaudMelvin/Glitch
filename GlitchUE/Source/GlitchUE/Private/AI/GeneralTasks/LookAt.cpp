// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/LookAt.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Kismet/KismetMathLibrary.h"

ULookAt::ULookAt(){
	LookAtPosition.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ULookAt, LookAtPosition));
	LookAtPosition.AddRotatorFilter(this, GET_MEMBER_NAME_CHECKED(ULookAt, LookAtPosition));
	LookAtPosition.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ULookAt, LookAtPosition), AActor::StaticClass());
}

void ULookAt::InitializeFromAsset(UBehaviorTree& Asset){
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset)){
		LookAtPosition.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type ULookAt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){

	const UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();
	AActor* CurrentActor = OwnerComp.GetOwner();
	FRotator TargetRotator;

	if(LookAtPosition.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()){
		TargetRotator = UKismetMathLibrary::FindLookAtRotation(CurrentActor->GetActorLocation(), CurrentBlackboard->GetValue<UBlackboardKeyType_Vector>(LookAtPosition.GetSelectedKeyID()));
	}

	else if(LookAtPosition.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass()){
		TargetRotator = CurrentBlackboard->GetValue<UBlackboardKeyType_Rotator>(LookAtPosition.GetSelectedKeyID()); 
	}

	else if(LookAtPosition.SelectedKeyType == UBlackboardKeyType_Object::StaticClass()){
		const AActor* ActorToLookAt = Cast<AActor>(CurrentBlackboard->GetValue<UBlackboardKeyType_Object>(LookAtPosition.GetSelectedKeyID()));
		TargetRotator = UKismetMathLibrary::FindLookAtRotation(CurrentActor->GetActorLocation(), ActorToLookAt->GetActorLocation());
	}

	if(bIgnorePitchRotation){
		TargetRotator.Pitch = 0;
	}

	CurrentActor->SetActorRotation(TargetRotator, ETeleportType::None);

	return EBTNodeResult::Succeeded;
}
