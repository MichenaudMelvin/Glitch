// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/SetPawnTransform.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Kismet/KismetMathLibrary.h"

USetPawnTransform::USetPawnTransform(){
	ForceInstancing(true);

	bNotifyTick = true;

	TargetLocation.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(USetPawnTransform, TargetLocation));
	TargetRotation.AddRotatorFilter(this, GET_MEMBER_NAME_CHECKED(USetPawnTransform, TargetRotation));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_ZeroToOneCurve"));
	check(Curve.Succeeded());

	ZeroToOneCurve = Curve.Object;

	FOnTimelineFloat UpdateEvent;
	FOnTimelineEvent FinishedEvent;

	UpdateEvent.BindDynamic(this, &USetPawnTransform::UpdateLerp);
	FinishedEvent.BindDynamic(this, &USetPawnTransform::FinishLerp);

	LerpTimeline.AddInterpFloat(ZeroToOneCurve, UpdateEvent);
	LerpTimeline.SetTimelineFinishedFunc(FinishedEvent);
}

EBTNodeResult::Type USetPawnTransform::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	LerpTimeline.SetPlayRate(1/TaskDuration);

	const UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();

	OwnerBehaviorTree = &OwnerComp;

	const AAIController* Controller = OwnerBehaviorTree->GetAIOwner();

	TargetPawn = Controller->GetPawn();

	CurrentLocation = TargetPawn->GetActorLocation();
	CurrentRotator = TargetPawn->GetActorRotation();

	TargetVector = CurrentBlackboard->GetValue<UBlackboardKeyType_Vector>(TargetLocation.SelectedKeyName);
	TargetRotator = CurrentBlackboard->GetValue<UBlackboardKeyType_Rotator>(TargetRotation.SelectedKeyName);

	LerpTimeline.PlayFromStart();

	return EBTNodeResult::InProgress;
}

void USetPawnTransform::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds){
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	LerpTimeline.TickTimeline(DeltaSeconds);
}

void USetPawnTransform::UpdateLerp(float Value){
	TargetPawn->SetActorLocation(UKismetMathLibrary::VLerp(CurrentLocation, TargetVector, Value));
	TargetPawn->SetActorRotation(UKismetMathLibrary::RLerp(CurrentRotator, TargetRotator, Value, false));
}

void USetPawnTransform::FinishLerp(){
	FinishLatentTask(*OwnerBehaviorTree, EBTNodeResult::Succeeded);
}
