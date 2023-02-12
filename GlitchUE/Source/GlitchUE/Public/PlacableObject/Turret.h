// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/PlacableActor.h"
#include "Components/TimelineComponent.h"
#include "Components/SphereComponent.h"
#include "Turret.generated.h"

class AMainAICharacter;

UENUM()
enum class FFocusMethod : uint8{
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* TurretPillar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh")
	USkeletalMeshComponent* TurretHead;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Vision")
	USphereComponent* TurretVision;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float Damages;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float FireRate;

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

	virtual void GlitchUpgrade() override;

	virtual void SetMesh() override;

	virtual void SetData(UPlacableActorData* NewData) override;

	bool CanSeeThroughWalls = false;

	FFocusMethod FocusMethod;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Attack();
	virtual void Attack_Implementation();

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
	AActor* GetFirstAI();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetMidAI();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetLastAI();

	UFUNCTION(BlueprintCallable)
	void SelectTarget();

	TArray<AActor*> GetSortedAIList();

	UPROPERTY(BlueprintReadOnly)
	TSet<AActor*> HittedAIList;

	FRotator AILookAtRotation;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool DoesAIListContainSomething();

	UFUNCTION()
	void OnReachTurretVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLeaveTurretVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle CanAttackTimer;

	virtual void SetObjectMaterial(UMaterialInterface* NewMaterial);
};
