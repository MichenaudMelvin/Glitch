// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIFocusCatalyseur/FocusCatalyseurController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Kismet/GameplayStatics.h"

AFocusCatalyseurController::AFocusCatalyseurController(){
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeAsset(TEXT("/Game/Blueprint/AI/FocusAI/AIFocusCatalyseur/BT_FocusCatalyseur"));
	check(BehaviorTreeAsset.Succeeded());

	BehaviorTree = BehaviorTreeAsset.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardAsset(TEXT("/Game/Blueprint/AI/FocusAI/AIFocusCatalyseur/BB_FocusCatalyseur"));
	check(BlackboardAsset.Succeeded());

	BlackboardData = BlackboardAsset.Object;
}

void AFocusCatalyseurController::BeginPlay(){
	Super::BeginPlay();

	TArray<AActor*> CatalyseurArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACatalyseur::StaticClass(), CatalyseurArray);
	for(int i = 0; i < CatalyseurArray.Num(); i++){
		CatalyseurList.Add(Cast<ACatalyseur>(CatalyseurArray[i]));
	}

	Blackboard->SetValueAsVector("NexusLocation", Nexus->GetActorLocation());
}

TArray<ACatalyseur*> AFocusCatalyseurController::GetCatalyseurList() const{
	return CatalyseurList;
}
