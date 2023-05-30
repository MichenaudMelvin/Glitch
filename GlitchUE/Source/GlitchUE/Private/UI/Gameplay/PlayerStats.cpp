// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/PlayerStats.h"
#include "Kismet/GameplayStatics.h"

void UPlayerStats::NativeConstruct(){
	Super::NativeConstruct();

	Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->OnSwitchPhases.AddDynamic(this, &UPlayerStats::SwitchPhases);
}

void UPlayerStats::SwitchPhases(EPhases NewPhase){
	if(NewPhase == EPhases::TowerDefense){
		WaveText->SetVisibility(ESlateVisibility::Visible);
		NexusHealth->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPlayerStats::UpdateDisplayGolds(int Golds) const{
	Golds = FMath::Clamp(Golds, 0, 999999999);
	GoldsText->SetText(FText::FromString(FString::FromInt(Golds)));
}

void UPlayerStats::UpdateWaveNumber(int CurrentWave){
	WaveText->SetText(FText::FromString("Current Wave: " + FString::FromInt(CurrentWave)));
}

void UPlayerStats::UpdateNexusHealth(const float NewHealth) const{
	NexusHealth->SetText(FText::FromString("Nexus Health: " + FString::FromInt(NewHealth)));
}

void UPlayerStats::UpdateObjectivesText(const FString NewObjective) const{
	ObjectiveText->SetText(FText::FromString(NewObjective));

}

void UPlayerStats::UpdateTutorialText(const FString NewTutorial) const{
	TutorialText->SetText(FText::FromString(NewTutorial));
}
