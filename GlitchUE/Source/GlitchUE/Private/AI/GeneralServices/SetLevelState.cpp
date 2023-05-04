// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralServices/SetLevelState.h"

#include "Kismet/GameplayStatics.h"

USetLevelState::USetLevelState(){
	Interval = 1;
	RandomDeviation = 0;
}

void USetLevelState::PostLoad(){
	Super::PostLoad();

#if WITH_EDITOR
	if(!IsValid(GetWorld())){
		return;
	}
#endif

	GlitchUEGameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void USetLevelState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds){
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	GlitchUEGameMode->SetLevelState(NewLevelState);
}
