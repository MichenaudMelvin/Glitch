// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PursuitDronePad.h"
#include "AI/MainAICharacter.h"
#include "PursuitDrone.generated.h"

UCLASS()
class GLITCHUE_API APursuitDrone : public AMainAICharacter{
	GENERATED_BODY()

public:
	APursuitDrone();

	float GetStartAnimDuration() const;

	UFUNCTION(BlueprintCallable)
	void PlayStartAnim(const bool bReverseAnim = false) const;

	void SetCurrentPad(APursuitDronePad* NewPad);

	void ForceInDock() const;

	void ForceStartAnim() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, Category = "Pad")
	APursuitDronePad* Pad;

	UAnimationAsset* StartAnim;

	USkeletalMesh* DroneMesh;

	USkeletalMesh* CrystalMesh;

	UPROPERTY(EditDefaultsOnly)
	UInteractableComponent* InteractableComp;

	UFUNCTION()
	void OnTouchSomething(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void StopPursuitBehavior();

	UFUNCTION()
	void DroneMeshBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DroneMeshEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void TransformIntoPowerUp();

	void TransformIntoDrone();

	FTimeline SpinTimeline;

	UCurveFloat* ZeroToOneCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Spin")
	float SpinSpeed = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Spin")
	float SpinOffset = 90;

	UPROPERTY(EditDefaultsOnly, Category = "Spin")
	float InteriorRotationOffset = 45;

	UPROPERTY(EditDefaultsOnly, Category = "Spin")
	FVector SpinScale = FVector(0.5, 0.5, 0.5);

	UFUNCTION()
	void Spin(float Value);

public:
	UFUNCTION()
	void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer);

	void EnableSpinBehavior();

	void DisableSpinBehavior();

	void AttachDrone(AActor* ActorToAttach, const FName SocketName);

	void DetachDrone();

	UInteractableComponent* GetInteractableComp() const;
};
