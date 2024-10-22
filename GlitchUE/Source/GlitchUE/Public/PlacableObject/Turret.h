// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/PlacableActor.h"
#include "Components/TimelineComponent.h"
#include "Components/SphereComponent.h"
#include "Turret.generated.h"

class AMainAICharacter;

UENUM()
enum class EFocusMethod : uint8{
	FirstTarget,
	MidTarget,
	LastTarget,
};

UCLASS(Abstract)
class GLITCHUE_API ATurret : public APlacableActor{
	GENERATED_BODY()

public:
	ATurret();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;

	virtual void OnCleanWorld(UWorld* World, bool bSessionEnded, bool bCleanupResources) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* TurretBase;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* TurretPillar;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* TurretHead;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Vision")
	USphereComponent* TurretVision;

	UPROPERTY(BlueprintReadWrite)
	AActor* CurrentTarget;

	float CurrentYawRotation;

	float CurrentPitchRotation;

	FTimeline RotateTimeline;

	UCurveFloat* ZeroToOneCurve;

	UFUNCTION(BlueprintCallable)
	virtual void LookAtTarget();

	UFUNCTION()
	virtual void RotateToTarget(float Alpha);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void EndRotate();
	virtual void EndRotate_Implementation();

	virtual void ReceiveGlitchUpgrade() override;

	virtual void ResetGlitchUpgrade() override;

	virtual void AttachDroneToPlacable(APursuitDrone* NewDrone) override;

	virtual void RemoveDrone(AMainPlayer* MainPlayer) override;

	virtual void SetMesh() override;

	virtual void SetData(UPlacableActorData* NewData) override;

	UStaticMeshComponent* FullMesh;

	virtual void Appear(const bool ReverseEffect, const FOnTimelineEvent AppearFinishEvent) override;

	virtual void FadeIn(float Alpha) override;

	virtual void EndAppearance() override;

	bool CanSeeThroughWalls = false;

	EFocusMethod FocusMethod;

	virtual void Attack_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void CanAttack();

	/// <summary>
	/// Execute when the turret finish the attack (can restart an attack or call FinishAttacking)
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void EndAttack();

	/// <summary>
	/// Execute if the turret cannot attack anymore (no ai in its range)
	/// </summary>
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void FinishAttacking();
	virtual void FinishAttacking_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Shoot();
	virtual void Shoot_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetFirstAI() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetMidAI() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetLastAI() const;

	UFUNCTION(BlueprintCallable)
	void SelectTarget();

	TArray<AActor*> GetSortedAIList() const;

	UPROPERTY(BlueprintReadOnly)
	TSet<AActor*> HittedAIList;

	FRotator AILookAtRotation;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool DoesAIListContainSomething() const;

	virtual void OnReachVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnLeaveVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle CanAttackTimer;

	virtual void SetObjectMaterial(UMaterialInterface* NewMaterial) override;
};
