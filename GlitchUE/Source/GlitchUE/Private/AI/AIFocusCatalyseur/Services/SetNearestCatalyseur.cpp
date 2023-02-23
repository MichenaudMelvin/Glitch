// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIFocusCatalyseur/Services/SetNearestCatalyseur.h"

#include "AI/AIFocusCatalyseur/FocusCatalyseurController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "GameFramework/Character.h"

USetNearestCatalyseur::USetNearestCatalyseur(){
	Interval = 1;
	RandomDeviation = 0;
	bCallTickOnSearchStart = true;

	NexusLocation.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(USetNearestCatalyseur, NexusLocation));
	NearestCatalyseurLocation.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(USetNearestCatalyseur, NearestCatalyseurLocation));
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
	
	const TArray<ACatalyseur*> CatalyseurList; Cast<AFocusCatalyseurController>(OwnerPawn->GetController())->GetCatalyseurList();
	const ACatalyseur* NearestCatalyseur = SetFirstCatalyseurToCheck(CatalyseurList);

	for (int i = 0; i < CatalyseurList.Num(); i++){
		if(CatalyseurList[i]->GetActivableComp()->IsActivated() && i != 0){
			const float CurrentDistance = CatalyseurList[i]->GetDistanceTo(Cast<AActor>(OwnerPawn));
			const float NearestDistance = NearestCatalyseur->GetDistanceTo(Cast<AActor>(OwnerPawn));
			if(CurrentDistance < NearestDistance){
				NearestCatalyseur = CatalyseurList[i];
			}
		}
	}

	CurrentBlackboard->SetValue<UBlackboardKeyType_Vector>(NearestCatalyseurLocation.GetSelectedKeyID(), NearestCatalyseur->GetActorLocation());
}

ACatalyseur* USetNearestCatalyseur::SetFirstCatalyseurToCheck(const TArray<ACatalyseur*> CatalyseurList){
	for (int i = 0; i < CatalyseurList.Num(); i++){
		if(CatalyseurList[i]->GetActivableComp()->IsActivated()){
			return CatalyseurList[i];
		}
	}

	return nullptr;
}
