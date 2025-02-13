// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIFocusCatalyseur/Services/SetNearestCatalyseur.h"

#include "AI/AIFocusCatalyseur/FocusCatalyseurController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "GameFramework/Character.h"

USetNearestCatalyseur::USetNearestCatalyseur(){
	Interval = 1;
	RandomDeviation = 0;
	bCallTickOnSearchStart = true;

	NexusLocation.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(USetNearestCatalyseur, NexusLocation));
	NearestCatalyseurLocation.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(USetNearestCatalyseur, NearestCatalyseurLocation));
	NearestCatalyseurKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(USetNearestCatalyseur, NearestCatalyseurKey), ACatalyseur::StaticClass());
}

void USetNearestCatalyseur::InitializeFromAsset(UBehaviorTree& Asset){
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset)){
		NexusLocation.ResolveSelectedKey(*BBAsset);
		NearestCatalyseurLocation.ResolveSelectedKey(*BBAsset);
	}
}

void USetNearestCatalyseur::OnSearchStart(FBehaviorTreeSearchData& SearchData){
	Super::OnSearchStart(SearchData);

	OwnerPawn = SearchData.OwnerComp.GetAIOwner()->GetCharacter();
}

void USetNearestCatalyseur::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds){
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();

	const AMainAIController* CurrentController = Cast<AMainAIController>(OwnerComp.GetAIOwner());

	if(!IsValid(CurrentController)){
		return;
	}

	const TArray<ACatalyseur*> CatalyseurList = CurrentController->GetCatalyseurList();
	ACatalyseur* NearestCatalyseur = SetFirstCatalyseurToCheck(CatalyseurList);

	for (int i = 0; i < CatalyseurList.Num(); i++){
		if(CatalyseurList[i]->GetActivableComp()->IsActivated() && i != 0){
			const float CurrentDistance = CatalyseurList[i]->GetDistanceTo(CurrentController->GetPawn());
			const float NearestDistance = NearestCatalyseur->GetDistanceTo(CurrentController->GetPawn());
			if(CurrentDistance < NearestDistance){
				NearestCatalyseur = CatalyseurList[i];
			}
		}
	}

	if(IsValid(NearestCatalyseur)){
		CurrentBlackboard->SetValue<UBlackboardKeyType_Vector>(NearestCatalyseurLocation.GetSelectedKeyID(), NearestCatalyseur->GetActorLocation());
		CurrentBlackboard->SetValue<UBlackboardKeyType_Object>(NearestCatalyseurKey.SelectedKeyName, NearestCatalyseur);
	}
}

ACatalyseur* USetNearestCatalyseur::SetFirstCatalyseurToCheck(const TArray<ACatalyseur*> CatalyseurList){
	for (int i = 0; i < CatalyseurList.Num(); i++){
		if(CatalyseurList[i]->GetActivableComp()->IsActivated()){
			return CatalyseurList[i];
		}
	}

	return nullptr;
}
