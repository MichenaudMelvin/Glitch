// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/AlertNearestAI.h"
#include "AI/MainAIController.h"
#include "AI/AIPursuitDrone/PursuitDrone.h"
#include "AI/AIPursuitDrone/PursuitDroneController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Helpers/FunctionsLibrary/UsefulFunctions.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UAlertNearestAI::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	AIList.Empty();

	TArray<AActor*> MainAIControllerArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APursuitDroneController::StaticClass(), MainAIControllerArray);

	for(int i = 0; i < MainAIControllerArray.Num(); i++){
		AIList.Add(Cast<APursuitDroneController>(MainAIControllerArray[i])->GetPawn());
	}

	AIList = UUsefulFunctions::SortActorsByDistanceToActor(AIList, OwnerComp.GetAIOwner()->GetPawn());

	int AICalled = 0;

	for(int i = 0; i < AIList.Num(); i++){
		UBlackboardComponent* CurrentBlackboardComp = Cast<APursuitDrone>(AIList[i])->GetBlackBoard();

		if(CurrentBlackboardComp->GetValueAsBool("IsLoading") || CurrentBlackboardComp->GetValueAsBool("ReceiveAlert")){
			continue;
		}

		AICalled++;

		CurrentBlackboardComp->SetValueAsBool("ReceiveAlert", true);

		if(NumberOfAIToCall == AICalled){
			break;
		}
	}

	return EBTNodeResult::Succeeded;
}
