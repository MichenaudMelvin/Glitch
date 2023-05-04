// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/InteractableComponent.h"
#include "Mark.generated.h"

UCLASS()
class GLITCHUE_API AMark : public AActor{
	GENERATED_BODY()

public:
	AMark();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile")
	UStaticMeshComponent* MarkMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	TArray<UStaticMesh*> PossibleMeshList;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	float SwitchMeshTime = 0.2f;

	void SwitchMesh();

	FTimerHandle SwitchMeshTimer;

	AMainPlayer* Player;

	FVector OriginalLocation;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interaction")
	UInteractableComponent* InteractableComp;

	UFUNCTION()
	void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer);

#pragma region Projectile

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovement;	

	void StartProjectile() const;

	void StopProjectile() const;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Projectile")
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

	UPROPERTY(EditDefaultsOnly, Category = "Distance")
	float MaxDistance;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Distance")
	float GetMaxDistance() const;

#pragma endregion
};
