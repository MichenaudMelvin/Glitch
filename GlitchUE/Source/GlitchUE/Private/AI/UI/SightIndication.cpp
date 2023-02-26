// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UI/SightIndication.h"
#include "Components/CanvasPanelSlot.h"

void USightIndication::NativeConstruct(){
	Super::NativeConstruct();
	
	TopSize = Cast<UCanvasPanelSlot>(Red->Slot)->GetOffsets().Top;
}

void USightIndication::UpdateSightIndication(const float Value){
	FMargin NewMargin;
	NewMargin.Top = FMath::Abs(((Value * TopSize) / 100) - TopSize);

	Cast<UCanvasPanelSlot>(Red->Slot)->SetOffsets(NewMargin);
}
