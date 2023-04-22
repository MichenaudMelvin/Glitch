// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralServices/UpdateValue.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"

UUpdateValue::UUpdateValue(){
	Interval = 1;
	RandomDeviation = 0;

	ValueToUpdate.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(UUpdateValue, ValueToUpdate));
	ValueToUpdate.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UUpdateValue, ValueToUpdate));
}

void UUpdateValue::InitializeFromAsset(UBehaviorTree& Asset){
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset)){
		ValueToUpdate.ResolveSelectedKey(*BBAsset);
	}
}

void UUpdateValue::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds){
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();

	// si la blackboard key est un int
	if (ValueToUpdate.SelectedKeyType == UBlackboardKeyType_Int::StaticClass()){
		int IntValue = CurrentBlackboard->GetValue<UBlackboardKeyType_Int>(ValueToUpdate.GetSelectedKeyID());

		switch (UpdateMethod){
		case EUpdateMethod::Increment:
			IntValue++;
			break;
		case EUpdateMethod::Decrement:
			IntValue--;
			break;
		}

		CurrentBlackboard->SetValue<UBlackboardKeyType_Int>(ValueToUpdate.GetSelectedKeyID(), IntValue);
	}

	// si la blackboard key est un float
	else if(ValueToUpdate.SelectedKeyType == UBlackboardKeyType_Float::StaticClass()){
		float FloatValue = CurrentBlackboard->GetValue<UBlackboardKeyType_Float>(ValueToUpdate.GetSelectedKeyID());

		switch (UpdateMethod){
		case EUpdateMethod::Increment:
			FloatValue++;
			break;
		case EUpdateMethod::Decrement:
			FloatValue--;
			break;
		}

		CurrentBlackboard->SetValue<UBlackboardKeyType_Float>(ValueToUpdate.GetSelectedKeyID(), FloatValue);
	}
}
