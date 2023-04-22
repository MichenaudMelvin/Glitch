// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralDecorators/CheckLevelState.h"
#include "Kismet/GameplayStatics.h"

bool UCheckLevelState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const{
	return Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetLevelState() == TargetLevelState;
}
