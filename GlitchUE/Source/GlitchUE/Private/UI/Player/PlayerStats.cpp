// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PlayerStats.h"

void UPlayerStats::UpdateDisplayGolds(int Golds) const{
	Golds = FMath::Clamp(Golds, 0, 999999999);
	GoldsText->SetText(FText::FromString(FString::FromInt(Golds)));
}
