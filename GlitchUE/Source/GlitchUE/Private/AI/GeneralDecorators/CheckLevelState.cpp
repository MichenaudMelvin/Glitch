// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralDecorators/CheckLevelState.h"
#include "Kismet/GameplayStatics.h"

void UCheckLevelState::PostLoad(){
	Super::PostLoad();

#if WITH_EDITOR
	if(!IsValid(GetWorld())){
		return;
	}
#endif

	GlitchUEGameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

bool UCheckLevelState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const{
	return GlitchUEGameMode->GetLevelState() == TargetLevelState;
}