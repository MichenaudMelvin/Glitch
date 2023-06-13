// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/WaypointIndication.h"
#include "Components/CanvasPanelSlot.h"

UWaypointIndication::UWaypointIndication(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
	static ConstructorHelpers::FObjectFinder<UTexture> OffscreenImage(TEXT("/Game/UI/Objectives/T_CatalyseurWaypointIconOffscreen"));
	check(OffscreenImage.Succeeded());

	ImageBrush.SetResourceObject(OffscreenImage.Object);
	ImageBrush.SetImageSize(FVector2D(124.25f, 109.75f));
}

void UWaypointIndication::NativeTick(const FGeometry& MyGeometry, float InDeltaTime){
	Super::NativeTick(MyGeometry, InDeltaTime);

	for(int i = 0; i < WaypointsList.Num(); i++){
		SetWaypointImageLayout(WaypointsList[i]->GetImage(), WaypointsList[i]->IsAtLeft());
	}
}

void UWaypointIndication::SetWaypointImageLayout(UImage* TargetImageWaypoint, const bool AtLeft) const{
	UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>(TargetImageWaypoint->Slot);

	ImageSlot->SetAnchors(AtLeft ? LeftAnchors : RightAnchors);
	ImageSlot->SetAlignment(AtLeft ? LeftAlignment : RightAlignment);
	ImageSlot->SetPosition(AtLeft ? LeftPosition : RightPosition);

	TargetImageWaypoint->SetRenderScale(FVector2D(AtLeft ? 1 : -1, 1));
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

		Cast<UCanvasPanelSlot>(NewImage->Slot)->SetAutoSize(true);

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
