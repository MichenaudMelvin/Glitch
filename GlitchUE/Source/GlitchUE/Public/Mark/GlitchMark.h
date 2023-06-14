// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mark/FakeMark.h"
#include "Mark/Mark.h"
#include "GlitchMark.generated.h"

UCLASS()
class GLITCHUE_API AGlitchMark : public AMark{
	GENERATED_BODY()

public:
	AGlitchMark();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UPopcornFXEmitterComponent* MarkFX;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Interaction")
	UInteractableComponent* InteractableComp;

	UPROPERTY()
	AFakeMark* FakeMark;

	AMainPlayer* Player;

#pragma region Player

	UPROPERTY(EditDefaultsOnly, Category = "Location")
	FVector RelativeLocation = FVector(-50, 0, 130);

	void AttachToPlayer();

	void DetachToPlayer();

	UFUNCTION(BlueprintCallable, Category = "Behavior")
	void ReattachToPlayer();

	UPROPERTY(EditAnywhere, Category = "Behavior")
	bool bCanBeAttached = true;

	UFUNCTION()
	void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer);

public:
	FVector GetTPLocation();

#pragma endregion

#pragma region Distance

protected:
	void DistanceTimer();

	FTimerHandle DistanceTimerHandle;

	FTimeline DistanceFromTheMarkTimeline;

	UPROPERTY(EditDefaultsOnly, Category = "Distance")
	float GoBackToPlayerDuration = 0.5f;

	FVector LastPosition;

	UFUNCTION()
	void DistanceFromMark(float Value);

	UPROPERTY(EditDefaultsOnly, Category = "Distance")
	float MaxLaunchDistance = 5000;

public:
	float GetMaxLaunchDistance() const;

#pragma endregion

#pragma region MarkMovement

	virtual void PlaceMark() override;

	virtual void ResetMark() override;

	virtual void Launch(const FRotator StartRotation) override;

#pragma endregion

	UProjectileMovementComponent* GetProjectileMovementComp() const;

	AFakeMark* GetFakeMark() const;
};
