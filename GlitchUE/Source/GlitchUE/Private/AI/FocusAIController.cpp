// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FocusAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AFocusAIController::BeginPlay(){
	Super::BeginPlay();

	Blackboard->SetValueAsFloat("AttackDistance", AttackDistance);

	TArray<AActor*> NexusArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANexus::StaticClass(), NexusArray);
	Nexus = Cast<ANexus>(NexusArray[0]);

	Blackboard->SetValueAsObject("Nexus", Nexus);
	Blackboard->SetValueAsVector("NexusLocation", Nexus->GetActorLocation());
}
