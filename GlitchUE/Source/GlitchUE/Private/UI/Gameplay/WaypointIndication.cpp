// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/WaypointIndication.h"
#include "Components/CanvasPanelSlot.h"

UWaypointIndication::UWaypointIndication(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
	//ImageBrush.SetResourceObject();
}

void UWaypointIndication::NativeTick(const FGeometry& MyGeometry, float InDeltaTime){
	Super::NativeTick(MyGeometry, InDeltaTime);

	for(int i = 0; i < WaypointsList.Num(); i++){
		SetWaypointImageLayout(WaypointsList[i]->GetImage(), WaypointsList[i]->IsAtLeft());
	}
}

void UWaypointIndication::SetWaypointImageLayout(const UImage* TargetImageWaypoint, const bool AtLeft) const{
	UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>(TargetImageWaypoint->Slot);

	ImageSlot->SetAnchors(AtLeft ? LeftAnchors : RightAnchors);
	ImageSlot->SetAlignment(AtLeft ? LeftAlignment : RightAlignment);
	ImageSlot->SetPosition(AtLeft ? LeftPosition : RightPosition);
}

void UWaypointIndication::AddIndication(UWaypoint* WaypointToAdd){
	if(IsWaypointInList(WaypointToAdd)){
		return;
	}

	if(!IsInViewport()){
		AddToViewport();
	}

	if(IsValid(WaypointToAdd->GetImage())){
		WaypointToAdd->GetImage()->SetVisibility(ESlateVisibility::Visible);
	} else{
		UImage* NewImage = NewObject<UImage>();
		WaypointToAdd->SetImage(NewImage);

		Cast<UCanvasPanel>(GetRootWidget())->AddChild(NewImage);

		SetWaypointImageLayout(NewImage, true);
		NewImage->SetBrush(ImageBrush);
	}

	WaypointsList.Add(WaypointToAdd);
}

void UWaypointIndication::RemoveIndication(UWaypoint* WaypointToRemove){
	if(!IsWaypointInList(WaypointToRemove)){
		return;
	}

	const int TargetIndex = WaypointsList.IndexOfByKey(WaypointToRemove);
	WaypointsList[TargetIndex]->GetImage()->SetVisibility(ESlateVisibility::Hidden);
	WaypointsList.RemoveAt(TargetIndex);

	if(WaypointsList.Num() == 0){
		RemoveFromParent();
	}
}

bool UWaypointIndication::IsWaypointInList(const UWaypoint* TargetWaypoint) const{
	return WaypointsList.Contains(TargetWaypoint);
}
