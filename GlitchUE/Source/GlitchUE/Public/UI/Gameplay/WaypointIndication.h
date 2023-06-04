// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Waypoint.h"
#include "WaypointIndication.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UWaypointIndication : public UUserWidget{
	GENERATED_BODY()

protected:
	UWaypointIndication(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	TArray<UWaypoint*> WaypointsList;

	FSlateBrush ImageBrush;

	void SetWaypointImageLayout(const UImage* TargetImageWaypoint, const bool AtLeft) const;

#pragma region LayoutVariables

	const FVector2D LeftAlignment = FVector2D(0.0f, 0.5f);

	const FAnchors LeftAnchors = FAnchors(0.0f, 0.5f, 0.0f, 0.5f);

	const FVector2D LeftPosition = FVector2D(50.0f, 0.0f);

	const FVector2D RightAlignment = FVector2D(1.0f, 0.5f);

	const FAnchors RightAnchors = FAnchors(1.0f, 0.5f, 1.0f, 0.5f);

	const FVector2D RightPosition = FVector2D(-50.0f, 0.0f);

#pragma endregion

public:
	UFUNCTION(BlueprintCallable, Category = "Waypoint")
	void AddIndication(UWaypoint* WaypointToAdd);

	UFUNCTION(BlueprintCallable, Category = "Waypoint")
	void RemoveIndication(UWaypoint* WaypointToRemove);

	UFUNCTION(BlueprintCallable, Category = "Waypoint")
	bool IsWaypointInList(const UWaypoint* TargetWaypoint) const;
};
