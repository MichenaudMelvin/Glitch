// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/AttackTarget.h"
#include "PopcornFXAttributeFunctions.h"
#include "AI/FocusAICharacter.h"
#include "AI/MainAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Components/HealthComponent.h"
#include "Kismet/KismetMathLibrary.h"

UAttackTarget::UAttackTarget(){
	TargetToAttack.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UAttackTarget, TargetToAttack), AActor::StaticClass());
	AttackKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UAttackTarget, AttackKey));
}

void UAttackTarget::InitializeFromAsset(UBehaviorTree& Asset){
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset)){
		TargetToAttack.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UAttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	CurrentOwnerComp = &OwnerComp;
	CurrentBlackboard = CurrentOwnerComp->GetBlackboardComponent();

	AMainAIController* AIController = Cast<AMainAIController>(OwnerComp.GetAIOwner());
	APawn* AIPawn = AIController->GetPawn();

	AActor* Target = Cast<AActor>(CurrentBlackboard->GetValue<UBlackboardKeyType_Object>(TargetToAttack.GetSelectedKeyID()));

	Attack(AIPawn, AIController, Target);

	return EBTNodeResult::InProgress;
}

void UAttackTarget::OnFinishAttackFX(UPopcornFXEmitterComponent* EmitterComponent, FVector Location, FVector Rotation){
	EmitterComponent->OnEmissionStops.AddDynamic(this, &UAttackTarget::OnFinishAttackFX);

	CurrentBlackboard->SetValue<UBlackboardKeyType_Bool>(AttackKey.SelectedKeyName, false);

	FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Succeeded);
}

void UAttackTarget::Attack(APawn* AIPawn, AMainAIController* AIController, AActor* Target){
	if(IsValid(Target->GetComponentByClass(UHealthComponent::StaticClass()))){
		UHealthComponent* TargetHealthComp = Cast<UHealthComponent>(Target->GetComponentByClass(UHealthComponent::StaticClass()));

		if(TargetHealthComp->GetCanTakeDamages()){
			PlayAttackFX(AIPawn, Target);

			CurrentBlackboard->SetValue<UBlackboardKeyType_Bool>(AttackKey.SelectedKeyName, true);
			const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(AIPawn->GetActorLocation(), Target->GetActorLocation());

			AIPawn->SetActorRotation(TargetRotation);

			TargetHealthComp->TakeDamages(AIController->GetDamages());
			return;
		}
	}

	FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Failed);
}

void UAttackTarget::PlayAttackFX(APawn* AIPawn, const AActor* Target){
	UPopcornFXEmitterComponent* AttackFX = Cast<AFocusAICharacter>(AIPawn)->GetAttackFX();

	const int TargetIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(AttackFX, "Target");

	UPopcornFXAttributeFunctions::SetAttributeAsVector(AttackFX, TargetIndex, Target->GetActorLocation(), true);

	if(!AttackFX->OnEmissionStops.IsAlreadyBound(this, &UAttackTarget::OnFinishAttackFX)){
		AttackFX->OnEmissionStops.AddDynamic(this, &UAttackTarget::OnFinishAttackFX);
	}

	AttackFX->StartEmitter();
}
