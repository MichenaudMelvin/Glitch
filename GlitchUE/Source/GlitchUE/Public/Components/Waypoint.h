// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Components/MaterialBillboardComponent.h"
#include "Player/MainPlayer.h"
#include "Waypoint.generated.h"

class AMainPlayerController;

UCLASS(ClassGroup = "Waypoint", meta = (BlueprintSpawnableComponent))
class GLITCHUE_API UWaypoint : public UMaterialBillboardComponent{
	GENERATED_BODY()

public:
	UWaypoint();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsVisibleOnScreen() const;

	AMainPlayerController* MainPlayerController;

	AMainPlayer* Player;

	UImage* ImageRef;

	void CheckComponentRender();

	UPROPERTY(EditDefaultsOnly, Category = "Waypoint")
	bool bDrawWayPoint = true;

public:
	bool IsAtLeft() const;

	void SetImage(UImage* NewImage);

	UImage* GetImage() const;

	UFUNCTION(BlueprintCallable, Category = "Waypoint")
	void DrawWaypoint(const bool bShouldDrawWaypoint);
};
