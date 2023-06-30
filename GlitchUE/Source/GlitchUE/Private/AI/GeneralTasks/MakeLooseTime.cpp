// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralTasks/MakeLooseTime.h"
#include "Kismet/GameplayStatics.h"

void UMakeLooseTime::PostLoad(){
	Super::PostLoad();

	if(IsValid(GetWorld())){
		GameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	}
}

EBTNodeResult::Type UMakeLooseTime::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){

	GameMode->RemoveStealthTime(LooseTime);

	return EBTNodeResult::Succeeded;
}
