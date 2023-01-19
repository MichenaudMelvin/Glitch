// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/PlacableActor.h"
#include "Components/TimelineComponent.h"
#include "Components/SphereComponent.h"
#include "Turret.generated.h"

class AMainAICharacter;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vision")
	USphereComponent* TurretRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float Damages;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float FireRate;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float Radius;

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

	UFUNCTION(BlueprintCallable)
	void CanAttack();

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle CanAttackTimer;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Attack();
	virtual void Attack_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetFirstAI();

	FRotator AILookAtRotation;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool DoesAIListContainSomething();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeginOverlap(AActor* OverlappedActor);
	void BeginOverlap_Implementation(AActor* OverlappedActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndOverlap(AActor* OverlappedActor);
	void EndOverlap_Implementation(AActor* OverlappedActor);

	virtual void SetObjectMaterial(UMaterialInterface* NewMaterial);
};
