// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FocusAIController.h"
#include "AI/FocusAIData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AFocusAIController::BeginPlay(){
	Super::BeginPlay();

	// maybe temp
	if(Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->OptionsString != ""){
		InitializeAIFromStart();
	}

	TArray<AActor*> NexusArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANexus::StaticClass(), NexusArray);
	Nexus = Cast<ANexus>(NexusArray[0]);
}

void AFocusAIController::InitializeAIFromStart(){
	Super::InitializeAIFromStart();

	const UFocusAIData* Data = Cast<UFocusAIData>(AIData);

	Blackboard->SetValueAsFloat("AttackDistance", Data->AttackDistance);

	Blackboard->SetValueAsObject("Nexus", Nexus);
	Blackboard->SetValueAsVector("NexusLocation", Nexus->GetActorLocation());
}
