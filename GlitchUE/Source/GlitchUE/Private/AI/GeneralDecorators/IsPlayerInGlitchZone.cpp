// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralDecorators/IsPlayerInGlitchZone.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Player/MainPlayer.h"

UIsPlayerInGlitchZone::UIsPlayerInGlitchZone(){
	Player.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UIsPlayerInGlitchZone, Player), AMainPlayer::StaticClass());
}

bool UIsPlayerInGlitchZone::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const{

	const UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();

#if WITH_EDITOR

	// for simulation mode
	if(!IsValid(CurrentBlackboard->GetValue<UBlackboardKeyType_Object>(Player.SelectedKeyName))){
		return false;
	}

#endif

	const UObject* PlayerObject = CurrentBlackboard->GetValue<UBlackboardKeyType_Object>(Player.SelectedKeyName);

	if(bInvertResult){
		return !Cast<AMainPlayer>(PlayerObject)->IsInGlitchZone();
	}

	return Cast<AMainPlayer>(PlayerObject)->IsInGlitchZone();
}