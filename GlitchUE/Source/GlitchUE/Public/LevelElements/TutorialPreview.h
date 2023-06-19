// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PopcornFXEmitterComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "TutorialPreview.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnReachFirstBox);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnReachSecondBox);

UCLASS()
class GLITCHUE_API ATutorialPreview : public AActor{
	GENERATED_BODY()

public:
	ATutorialPreview();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnCleanWorld(UWorld* World, bool bSessionEnded, bool bCleanupResources);

	UPROPERTY(EditDefaultsOnly, Category = "Spline")
	USplineComponent* Spline;

	UPROPERTY(EditDefaultsOnly, Category = "Wisp")
	UPopcornFXEmitterComponent* Wisp;

	UFUNCTION(BlueprintCallable, Category = "Spline")
	void FollowSpline();

	UFUNCTION(BlueprintCallable, Category = "Spline")
	void StopFollow();

	UPROPERTY(EditDefaultsOnly, Category = "TriggerBox")
	UBoxComponent* FirstBoxComp;

	UPROPERTY(EditDefaultsOnly, Category = "TriggerBox")
	UBoxComponent* SecondBoxComp;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates")
	FKOnReachFirstBox OnReachFirstBox;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates")
	FKOnReachSecondBox OnReachSecondBox;

	UFUNCTION()
	void FollowSplineUpdate(float Alpha);

	UFUNCTION()
	void FollowSplineFinish();

	FTimeline FollowTimeline;

	bool bLoopStarted = false;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	float SplineSpeed = 2.5f;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	float LoopDelay = 1.0f;

	FTimerHandle LoopDelayTimerHandle;

	UFUNCTION()
	void OverlapTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
