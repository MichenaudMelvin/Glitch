// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SightComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLooseSightPlayer, float, InvestigateTimePercent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSightPlayer, float, InvestigateTimePercent);

UCLASS(meta = (BlueprintSpawnableComponent))
class GLITCHUE_API USightComponent : public UStaticMeshComponent{
	GENERATED_BODY()

public:
	USightComponent();

protected:
	virtual void BeginPlay() override;

	void Check();

	void LooseSight();

	UBlackboardComponent* OwnerBlackboard;

	float InvestigateTimeElapsed = 0;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MaxInvestigateTime = 2;

	// can only be the player
	AActor* SightActor;

	bool bPlayerInCollision;

	FTimerHandle SightTimer;

	FTimerHandle LooseSightTimer;

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnSightPlayer OnSightPlayer;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnLooseSightPlayer OnLooseSightPlayer;

protected:
	UFUNCTION()
	void EnterSight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ExitSight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsSomethingInSight() const;

	bool bIsPlayerInSight = false;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsPlayerInSight() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetTimerElapsed() const;
};
