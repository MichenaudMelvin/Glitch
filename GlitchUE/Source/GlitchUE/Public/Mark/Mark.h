// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PopcornFXEmitterComponent.h"
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

	void AttachToPlayer();

	void DetachToPlayer();

	UPROPERTY(EditAnywhere, Category = "Behavior")
	bool bCanBeAttached = true;

	UPROPERTY(EditDefaultsOnly, Category = "Location")
	FVector RelativeLocation = FVector(-50, 0, 130);

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UStaticMeshComponent* MarkMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UPopcornFXEmitterComponent* MarkFX;

	AMainPlayer* Player;

	FVector OriginalLocation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Interaction")
	UInteractableComponent* InteractableComp;

	UFUNCTION()
	void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer);

	UFUNCTION(BlueprintCallable, Category = "Behavior")
	void ReattachToPlayer();

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
	void DistanceTimer();

	FTimerHandle DistanceTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Distance")
	float GoBackToPlayerDuration = 0.5f;

	FTimeline DistanceFromTheMarkTimeline;

	FVector LastPosition;

	UFUNCTION()
	void DistanceFromMark(float Value);

	UPROPERTY(EditDefaultsOnly, Category = "Distance")
	float MaxLaunchDistance = 5000;

public:
	float GetMaxLaunchDistance() const;

#pragma endregion
};
