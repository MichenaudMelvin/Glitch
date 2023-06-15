// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/PlayerStats.h"
#include "Kismet/GameplayStatics.h"

void UPlayerStats::UpdateDisplayGolds(int Golds){
	Golds = FMath::Clamp(Golds, 0, 999999999);
	GoldsText->SetText(FText::FromString(FString::FromInt(Golds)));
}

void UPlayerStats::UpdateNexusHealth(const float NewHealth, const float NexusMaxHealth) const{
	NexusHealth->SetText(FText::FromString(FString::FromInt(NewHealth)));
	NexusHealthBar->SetPercent(NewHealth / NexusMaxHealth);
}

void UPlayerStats::UpdateObjectivesText(const FString NewObjective) const{
	ObjectiveText->SetText(FText::FromString(NewObjective));

}

void UPlayerStats::UpdateAdditionalText(const FString NewMessage) const{
	AdditionalText->SetText(FText::FromString(NewMessage));
}
