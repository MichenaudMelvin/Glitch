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

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UStaticMeshComponent* MarkMesh;

#pragma region Projectile

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovement;

	void StartProjectile() const;

	void StopProjectile() const;

	bool LocationTrace(const float UpTraceValue, FVector& OutImpactPoint);

	bool bIsMarkPlaced = false;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Projectile")
	bool GetIsMarkPlaced() const;

#pragma endregion

#pragma region MarkMovement

	UFUNCTION(BlueprintCallable, Category = "MarkMovement")
	virtual void PlaceMark();

	UFUNCTION(BlueprintCallable, Category = "MarkMovement")
	virtual void ResetMark();

	UFUNCTION(BlueprintCallable, Category = "MarkMovement")
	virtual void Launch(const FRotator StartRotation);

#pragma endregion
};
