// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/AlertNearestAI.h"
#include "AI/MainAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "Kismet/GameplayStatics.h"

UAlertNearestAI::UAlertNearestAI(){
	Player.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UAlertNearestAI, Player), AActor::StaticClass());
}

void UAlertNearestAI::InitializeFromAsset(UBehaviorTree& Asset){
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset)){
		Player.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UAlertNearestAI::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	const UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();
	AIList.Empty();

	TArray<AActor*> MainAIControllerArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainAIController::StaticClass(), MainAIControllerArray);

	for(int i = 0; i < MainAIControllerArray.Num(); i++){
		AIList.Add(Cast<AMainAIController>(MainAIControllerArray[i])->GetPawn());
	}

	UUsefullFunctions::SortActorsByDistanceToActor(AIList, OwnerComp.GetAIOwner()->GetPawn());

	const FVector PlayerLocation = Cast<AActor>(CurrentBlackboard->GetValue<UBlackboardKeyType_Object>(Player.GetSelectedKeyID()))->GetActorLocation();

	for(int i = 0; i < NumberOfAIToCall; i++){
		if(IsValid(AIList[i])){
			UBlackboardComponent* CurrentBlackboardComp = Cast<AMainAIController>(AIList[i])->GetBlackboardComponent();
			CurrentBlackboardComp->SetValueAsVector("AlertedLocation", PlayerLocation);
			CurrentBlackboardComp->SetValueAsBool("ReceiveAlert", true);
		}
	}

	return EBTNodeResult::Succeeded;
}
