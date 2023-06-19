// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PursuitDronePad.h"
#include "AI/MainAICharacter.h"
#include "Components/CompassIcon.h"
#include "PursuitDrone.generated.h"

UCLASS(Abstract)
class GLITCHUE_API APursuitDrone : public AMainAICharacter{
	GENERATED_BODY()

public:
	APursuitDrone();

	float GetStartAnimDuration() const;

	UFUNCTION(BlueprintCallable)
	void PlayStartAnim(const bool bReverseAnim = false);

	void SetCurrentPad(APursuitDronePad* NewPad);

	void ForceInDock() const;

	void ForceStartAnim() const;

	virtual void SetCurrentData(UMainAIData* NewData) override;

	UCompassIcon* GetCompassIcon() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void OnCleanWorld(UWorld* World, bool bSessionEnded, bool bCleanupResources) override;

	UPROPERTY(EditAnywhere, Category = "Pad")
	APursuitDronePad* Pad;

	UPROPERTY(EditDefaultsOnly, Category = "Compass")
	UCompassIcon* CompassIcon;

	UAnimationAsset* StartAnim;

	FTimerHandle ReverseAnimTimerHandle;

	USkeletalMesh* CrystalMesh;

	UPROPERTY(EditDefaultsOnly)
	UInteractableComponent* InteractableComp;

	/**
	 * @brief (Timer in second)
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	float LoadingTime = 10.0f;

	FTimerHandle LoadingTimerHandle;

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

	FTimerHandle DetachTimerHandle;

	UCurveFloat* ZeroToOneCurve;

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
