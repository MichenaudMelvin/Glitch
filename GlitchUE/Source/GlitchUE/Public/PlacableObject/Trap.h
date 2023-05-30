// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/PlacableActor.h"
#include "Components/ActivableComponent.h"
#include "Components/BoxComponent.h"
#include "Trap.generated.h"

class UTrapData;

UENUM(BlueprintType)
enum class ETrapEffect : uint8{
	None,
	Burned,
	Frozen,
	Poisoned,
	SlowedDown,
};

UCLASS(Abstract)
class GLITCHUE_API ATrap : public APlacableActor{
	GENERATED_BODY()

public:
	ATrap();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* TrapMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* CrystalMesh;

	UAnimSequenceBase* CrystalAnimation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UActivableComponent* ActivableComp;

	UPROPERTY(VisibleDefaultsOnly)
	UBoxComponent* TrapDistance;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UPopcornFXEmitterComponent* IdleFX;

	float TrapDuration;

	ETrapEffect TrapEffect;

	float TrapEffectDuration;

	FTimerHandle ReactivationTimerHandle;

	float ReactivationTimer;

	UFUNCTION()
	void OnActivateTrap();

	UFUNCTION()
	void OnDesactivateTrap();

	virtual void ReceiveGlitchUpgrade() override;

	virtual void ResetGlitchUpgrade() override;

	virtual void AttachDroneToPlacable(APursuitDrone* NewDrone) override;

	virtual void RemoveDrone(AMainPlayer* MainPlayer) override;

	virtual void SetMesh() override;

	virtual void SetData(UPlacableActorData* NewData) override;

	virtual void Appear(const bool ReverseEffect, const FOnTimelineEvent AppearFinishEvent) override;

	virtual void FadeIn(float Alpha) override;

	void CheckCanAttack();

	virtual void Attack_Implementation() override;

	virtual void OnReachVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
