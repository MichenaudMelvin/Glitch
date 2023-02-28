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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile")
	UStaticMeshComponent* MarkMesh;

	AMainPlayer* Player;

	FVector OriginalLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
	UInteractableComponent* InteractableComp;

	UFUNCTION()
	void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer);

#pragma region Projectile

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovement;	

	void StartProjectile() const;

	void StopProjectile() const;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Projectile")
	FVector GetTPLocation();

protected:
	bool LocationTrace(const float UpTraceValue, FVector& OutImpactPoint);

	FVector LaunchLocation;

	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

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

	void CheckDistance();

	FTimerHandle DistanceTimer;

	UPROPERTY(EditAnywhere, Category = "Distance")
	float MaxDistance;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Distance")
	float GetMaxDistance();

#pragma endregion
};
