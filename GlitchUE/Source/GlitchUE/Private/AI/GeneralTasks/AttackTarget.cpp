// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/AttackTarget.h"
#include "AI/MainAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Components/HealthComponent.h"

UAttackTarget::UAttackTarget(){
	TargetToAttack.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UAttackTarget, TargetToAttack), AActor::StaticClass());
}

void UAttackTarget::InitializeFromAsset(UBehaviorTree& Asset){
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset)){
		TargetToAttack.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UAttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	const UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();

	const AActor* Target = Cast<AActor>(CurrentBlackboard->GetValue<UBlackboardKeyType_Object>(TargetToAttack.GetSelectedKeyID()));

	if(IsValid(Target->GetComponentByClass(UHealthComponent::StaticClass()))){
		UHealthComponent* TargetHealthComp = Cast<UHealthComponent>(Target->GetComponentByClass(UHealthComponent::StaticClass()));

		if(TargetHealthComp->GetCanTakeDamages()){
			TargetHealthComp->TakeDamages(Cast<AMainAIController>(OwnerComp.GetAIOwner())->GetDamages());
			return EBTNodeResult::Succeeded;
		}

		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
