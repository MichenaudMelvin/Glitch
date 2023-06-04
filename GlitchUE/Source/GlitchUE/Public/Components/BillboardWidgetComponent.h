// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "BillboardWidgetComponent.generated.h"

UCLASS(ClassGroup= "UserInterface", meta = (BlueprintSpawnableComponent))
class GLITCHUE_API UBillboardWidgetComponent : public UWidgetComponent{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category = "UserInterface", meta = (EditCondition = "Space == EWidgetSpace::World"))
	bool bEnableBillboardBehavior = true;

	APlayerCameraManager* CameraManager;

	void SelectRotation();
};
