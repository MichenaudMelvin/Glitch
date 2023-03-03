// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/PlacableActor.h"
#include "Components/ActivableComponent.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimationAsset.h"
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

UCLASS()
class GLITCHUE_API ATrap : public APlacableActor{
	GENERATED_BODY()

public:
	ATrap();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USkeletalMeshComponent* CrystalMesh;

	UAnimationAsset* CrystalAnimation;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UActivableComponent* ActivableComp;

	UBoxComponent* TrapDistance;

	UPopcornFXEmitterComponent* IdleFX;
	
	float Damages;
	
	float TrapDuration;
	
	float TrapAttackRate;
	
	ETrapEffect TrapEffect;

	float TrapEffectDuration;

	UFUNCTION()
	void OnActivateTrap();

	UFUNCTION()
	void OnDesactivateTrap();

	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer) override;
	
	virtual void ReciveGlitchUpgrade() override;
	
	virtual void SetData(UPlacableActorData* NewData) override;
	
	virtual void Attack_Implementation() override;
	
	virtual void OnReachVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void OnLeaveVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
