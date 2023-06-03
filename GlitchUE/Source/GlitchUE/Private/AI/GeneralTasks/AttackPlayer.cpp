// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/AttackPlayer.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/MainPlayer.h"

void UAttackPlayer::Attack(APawn* AIPawn, AMainAIController* AIController, AActor* Target){
	PlayAttackFX(AIPawn, Target);

	CurrentBlackboard->SetValue<UBlackboardKeyType_Bool>(AttackKey.SelectedKeyName, true);

	AMainPlayer* Player = Cast<AMainPlayer>(Target);
	Player->UpdateGolds(AIController->GetDamages(), EGoldsUpdateMethod::TakeDamages);

	const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(AIPawn->GetActorLocation(), Player->GetActorLocation());

	AIPawn->SetActorRotation(TargetRotation);
}
