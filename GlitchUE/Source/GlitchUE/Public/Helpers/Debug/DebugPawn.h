// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Player/MainPlayerController.h"
#include "DebugPawn.generated.h"

class UFloatingPawnMovement;

UCLASS()
class GLITCHUE_API ADebugPawn : public APawn{
	GENERATED_BODY()

public:
	ADebugPawn();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void MoveForward(float AxisValue);

	UFUNCTION()
	void MoveRight(float AxisValue);

	UFUNCTION()
	void PanPressed();

	UFUNCTION()
	void PanReleassed();

	UFUNCTION()
	void LookUp(float AxisValue);

	UFUNCTION()
	void Turn(float AxisValue);

	UFUNCTION()
	void ManageSpeed(float AxisValue);
	
	AMainPlayerController* MainPlayerController;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* SceneComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UFloatingPawnMovement* MovementComponent;
};
