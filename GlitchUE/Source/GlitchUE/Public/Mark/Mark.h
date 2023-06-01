// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/InteractableComponent.h"
#include "Components/TimelineComponent.h"
#include "Mark.generated.h"

UCLASS()
class GLITCHUE_API AMark : public AActor{
	GENERATED_BODY()

public:
	AMark();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UStaticMeshComponent* MarkMesh;

	FTimerHandle SwitchMeshTimer;

	AMainPlayer* Player;

	FVector OriginalLocation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Interaction")
	UInteractableComponent* InteractableComp;

	UFUNCTION()
	void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer);

#pragma region Projectile

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovement;

	void StartProjectile() const;

	void StopProjectile() const;

public:
	FVector GetTPLocation();

	void SetTargetLocation(const FVector NewTargetLocation);

	void SetHitSomething(const bool bValue);

protected:
	bool LocationTrace(const float UpTraceValue, FVector& OutImpactPoint);

	FVector LaunchLocation;

	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FVector TargetLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float EqualTolerance = 100;

	bool bShouldMarkHitSomething = false;

public:
	bool bIsMarkPlaced = false;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Projectile")
	bool GetIsMarkPlaced() const;

#pragma endregion

#pragma region MarkMovement

	UFUNCTION(BlueprintCallable, Category = "MarkMovement")
	void PlaceMark();

	UFUNCTION(BlueprintCallable, Category = "MarkMovement")
	void ResetMark();

	UFUNCTION(BlueprintCallable, Category = "MarkMovement")
	void Launch(const FTransform StartTransform);

#pragma endregion

#pragma region Distance

protected:
	float GetDistanceToLaunchPoint() const;

	void LaunchTimer();

	FTimerHandle LaunchTimerHandle;

	FTimerHandle DistanceFromTheMarkTimerHandle;

	void CheckDistanceFromMark();

	UPROPERTY(EditDefaultsOnly, Category = "Distance")
	float GoBackToPlayerDuration = 0.5f;

	FTimeline DistanceFromTheMarkTimeline;

	FVector LastPosition;

	UFUNCTION()
	void DistanceFromMark(float Value);

	UPROPERTY(EditDefaultsOnly, Category = "Distance")
	float MaxLaunchDistance = 5000;

	UPROPERTY(EditDefaultsOnly, Category = "Distance")
	float MaxDistanceFromTheMark = 5000;

public:
	float GetMaxLaunchDistance() const;

#pragma endregion
};
