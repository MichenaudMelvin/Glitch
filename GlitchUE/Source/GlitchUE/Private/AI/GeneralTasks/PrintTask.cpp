// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/PrintTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Class.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Name.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_NativeEnum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UPrintTask::UPrintTask(){}

void UPrintTask::InitializeFromAsset(UBehaviorTree& Asset){
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset)){
		VariableToPrint.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UPrintTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	const UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();

	if(VariableToPrint.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass()){
		const bool bBoolToPrint = CurrentBlackboard->GetValue<UBlackboardKeyType_Bool>(VariableToPrint.GetSelectedKeyID());
		UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), ( bBoolToPrint ? TEXT("true") : TEXT("false") ));
	}

	else if(VariableToPrint.SelectedKeyType == UBlackboardKeyType_Class::StaticClass()){
		const UClass* ClassToPrint = CurrentBlackboard->GetValue<UBlackboardKeyType_Class>(VariableToPrint.GetSelectedKeyID());
		UE_LOG(LogTemp, Warning, TEXT("The class name is %p"), ClassToPrint);
	}

	else if(VariableToPrint.SelectedKeyType == UBlackboardKeyType_Enum::StaticClass()){
		const uint8 EnumToPrint = CurrentBlackboard->GetValue<UBlackboardKeyType_Enum>(VariableToPrint.GetSelectedKeyID());
		UE_LOG(LogTemp, Warning, TEXT("The enum value is %hhu"), EnumToPrint);
	}

	else if(VariableToPrint.SelectedKeyType == UBlackboardKeyType_Float::StaticClass()){
		const float FloatToPrint = CurrentBlackboard->GetValue<UBlackboardKeyType_Float>(VariableToPrint.GetSelectedKeyID());
		UE_LOG(LogTemp, Warning, TEXT("The float value is %f"), FloatToPrint);
	}

	else if(VariableToPrint.SelectedKeyType == UBlackboardKeyType_Int::StaticClass()){
		const int IntToPrint = CurrentBlackboard->GetValue<UBlackboardKeyType_Int>(VariableToPrint.GetSelectedKeyID());
		UE_LOG(LogTemp, Warning, TEXT("The int name is %d"), IntToPrint);
	}

	else if(VariableToPrint.SelectedKeyType == UBlackboardKeyType_Name::StaticClass()){
		const FName NameToPrint = CurrentBlackboard->GetValue<UBlackboardKeyType_Name>(VariableToPrint.GetSelectedKeyID());
		UE_LOG(LogTemp, Warning, TEXT("The name value is %s"), *NameToPrint.ToString());
	}

	else if(VariableToPrint.SelectedKeyType == UBlackboardKeyType_NativeEnum::StaticClass()){
		const uint8 NativeEnumToPrint = CurrentBlackboard->GetValue<UBlackboardKeyType_NativeEnum>(VariableToPrint.GetSelectedKeyID());
		UE_LOG(LogTemp, Warning, TEXT("The native enum value is %hhu"), NativeEnumToPrint);
	}

	else if(VariableToPrint.SelectedKeyType == UBlackboardKeyType_Object::StaticClass()){
		const UObject* ObjectToPrint = CurrentBlackboard->GetValue<UBlackboardKeyType_Object>(VariableToPrint.GetSelectedKeyID());
		UE_LOG(LogTemp, Warning, TEXT("The object value is %s"), *ObjectToPrint->GetName());
	}

	else if(VariableToPrint.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass()){
		const FRotator RotatorToPrint = CurrentBlackboard->GetValue<UBlackboardKeyType_Rotator>(VariableToPrint.GetSelectedKeyID());
		UE_LOG(LogTemp, Warning, TEXT("The rotator value is %s"), *RotatorToPrint.ToString());
	}

	else if(VariableToPrint.SelectedKeyType == UBlackboardKeyType_String::StaticClass()){
		const FString StringToPrint = CurrentBlackboard->GetValue<UBlackboardKeyType_String>(VariableToPrint.GetSelectedKeyID());
		UE_LOG(LogTemp, Warning, TEXT("The string value is %s"), *StringToPrint);
	}

	else if(VariableToPrint.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()){
		const FVector VectorToPrint = CurrentBlackboard->GetValue<UBlackboardKeyType_Vector>(VariableToPrint.GetSelectedKeyID());
		UE_LOG(LogTemp, Warning, TEXT("The vector value is %s"), *VectorToPrint.ToString());
	}

	return EBTNodeResult::Succeeded;
}
