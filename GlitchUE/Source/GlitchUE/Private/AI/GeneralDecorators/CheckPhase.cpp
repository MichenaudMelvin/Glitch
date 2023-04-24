// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralDecorators/CheckPhase.h"
#include "Kismet/GameplayStatics.h"

UCheckPhase::UCheckPhase(){
	NodeName = "CheckCurrentPhase";
}

void UCheckPhase::PostLoad(){
	Super::PostLoad();

#if WITH_EDITOR
	if(!IsValid(GetWorld())){
		return;
	}
#endif

	GlitchUEGameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

bool UCheckPhase::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const{
	return GlitchUEGameMode->GetPhases() == TargetPhase;
}
