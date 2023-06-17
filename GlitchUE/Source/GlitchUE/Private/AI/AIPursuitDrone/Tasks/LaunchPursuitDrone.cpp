// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPursuitDrone/Tasks/LaunchPursuitDrone.h"
#include "AI/AIPursuitDrone/PursuitDrone.h"

ULaunchPursuitDrone::ULaunchPursuitDrone(){
	// for some reason I need this
	ForceInstancing(true);

	AnimsTimer = FTimerDelegate::CreateUObject(this, &ULaunchPursuitDrone::FinishAnims);
}

EBTNodeResult::Type ULaunchPursuitDrone::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){

	TimerHandle.Invalidate();

	OwnerBehaviorTree = &OwnerComp;

	const AAIController* Controller = OwnerBehaviorTree->GetAIOwner();

	APursuitDrone* Drone = Cast<APursuitDrone>(Controller->GetPawn());

	if(bSkipAnim){
		bLaunch ? Drone->ForceStartAnim() : Drone->ForceInDock();
		return EBTNodeResult::Succeeded;
	}

	Drone->PlayStartAnim(!bLaunch);

	Drone->GetIdleFX()->SetVisibility(bLaunch);

	Drone->GetCompassIcon()->SetAllowDraw(bLaunch);

	Drone->GetWorld()->GetTimerManager().SetTimer(TimerHandle, AnimsTimer, 1, false);

	return EBTNodeResult::InProgress;
}

void ULaunchPursuitDrone::FinishAnims(){
	FinishLatentTask(*OwnerBehaviorTree, EBTNodeResult::Succeeded);
}
