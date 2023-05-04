// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlitchInterface.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainAIPawn.generated.h"

UCLASS(Abstract)
class GLITCHUE_API AMainAIPawn : public APawn, public IGlitchInterface{
	GENERATED_BODY()

public:
	AMainAIPawn();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* AIMesh;

	UBlackboardComponent* Blackboard;

	virtual void ReceiveGlitchUpgrade() override;

	virtual void ResetGlitchUpgrade() override;
};
