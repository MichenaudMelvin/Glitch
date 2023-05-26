// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/AttackPlayer.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
#include "AI/MainAIController.h"
#include "Player/MainPlayer.h"

EBTNodeResult::Type UAttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();

	const AMainAIController* AIController = Cast<AMainAIController>(OwnerComp.GetAIOwner());
	APawn* AIPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());

	AMainPlayer* Player = Cast<AMainPlayer>(CurrentBlackboard->GetValue<UBlackboardKeyType_Object>(TargetToAttack.GetSelectedKeyID()));

	CurrentBlackboard->SetValue<UBlackboardKeyType_Bool>(AttackKey.SelectedKeyName, true);

	Player->UpdateGolds(AIController->GetDamages(), EGoldsUpdateMethod::TakeDamages);

	const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(AIPawn->GetActorLocation(), Player->GetActorLocation());

	AIPawn->SetActorRotation(TargetRotation);

	return EBTNodeResult::Succeeded;
}
