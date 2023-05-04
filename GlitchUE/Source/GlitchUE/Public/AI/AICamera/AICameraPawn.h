// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/MainAIPawn.h"
#include "Components/SightComponent.h"
#include "Components/WidgetComponent.h"
#include "AICameraPawn.generated.h"

UENUM(BlueprintType)
enum class ECameraBehavior : uint8{
	Static,
	Clamped,
	FullRotation,
};

UCLASS(Abstract)
class GLITCHUE_API AAICameraPawn : public AMainAIPawn{
	GENERATED_BODY()

public:
	AAICameraPawn();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USightComponent* SightComp;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UWidgetComponent* SightWidget;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Behavior")
	ECameraBehavior CameraBehavior;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Behavior", meta = (EditCondition = "CameraBehavior != ECameraBehavior::Static"))
	float SpeedRotation = 1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Behavior", meta = (EditCondition = "CameraBehavior == ECameraBehavior::Clamped"))
	float MaxRotation = 90;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Behavior", meta = (EditCondition = "CameraBehavior == ECameraBehavior::Clamped"))
	float MinRotation = -90;
};
