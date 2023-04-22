// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralDecorators/CheckPhase.h"
#include "Kismet/GameplayStatics.h"

UCheckPhase::UCheckPhase(){
	NodeName = "CheckCurrentPhase";
}

bool UCheckPhase::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const{
	return Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetPhases() == TargetPhase;
}
