// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SightComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Pawn.h"
#include "AICameraPawn.generated.h"

UENUM(BlueprintType)
enum class ECameraBehavior : uint8{
	Static,
	Clamped,
	FullRotation,
};

UCLASS()
class GLITCHUE_API AAICameraPawn : public APawn{
	GENERATED_BODY()

public:
	AAICameraPawn();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComp;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USightComponent* SightComp;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UWidgetComponent* SightWidget;
	
	UBlackboardComponent* Blackboard;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Behavior")
	ECameraBehavior CameraBehavior;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Behavior", meta = (EditCondition = "CameraBehavior != ECameraBehavior::Static"))
	float SpeedRoation = 1;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Behavior", meta = (EditCondition = "CameraBehavior == ECameraBehavior::Clamped"))
	float MaxRotation = 90;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Behavior", meta = (EditCondition = "CameraBehavior == ECameraBehavior::Clamped"))
	float MinRotation = -90;
};
