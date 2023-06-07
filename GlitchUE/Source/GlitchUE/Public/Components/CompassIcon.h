// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/MainPlayer.h"
#include "CompassIcon.generated.h"

class UCompassComponent;

UCLASS(ClassGroup= "Compass", meta=(BlueprintSpawnableComponent))
class GLITCHUE_API UCompassIcon : public UActorComponent{
	GENERATED_BODY()

public:
	UCompassIcon();

protected:
	virtual void BeginPlay() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMesh* OwnerMesh;

	FVector OwnerLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Appearance")
	bool bAllowDraw = true;

	/**
	 * @brief if the value is less than 0 the sprite can be always drawn
	 */
	UPROPERTY(EditAnywhere, Category = "Distance")
	float DrawDistance = 500;

	/**
	 * @brief if UseDynamicScale is set as true the icon scale will lerp from 0 to TargetScale
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Appearance")
	float TargetScale = 0.25f;

	/**
	 * @brief Auto setup should be used if there only one compass owner for the target class (ig : a player)
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	bool bSetupAuto = true;

	/**
	 * @brief if the icon owner is static it can be unchecked to improve performances
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	bool bTickDraw = true;

	UPROPERTY(EditDefaultsOnly, Category = "Behavior", meta = (EditCondition = "bTickDraw == true"))
	bool bUseDynamicScale = false;

	UPROPERTY(EditDefaultsOnly, Category = "Class")
	TSubclassOf<AActor> TargetCompassOwnerClass = AMainPlayer::StaticClass();

	UCompassComponent* CompassOwner;

public:
	UStaticMesh* GetOwnerMesh() const;

	FVector GetOwnerLocation() const;

	bool CanBeDrawn() const;

	void SetAllowDraw(const bool bAllow);

	FVector GetTargetScale() const;

	void SetDrawDistance(const float NewDistance);

	float GetDrawDistance() const;

	void SetOwnerClass(const TSubclassOf<AActor> NewOwnerClass);

	void SetupAuto(const bool bAuto);

	void SetShouldUseTick(const bool bShouldUseTick);

	bool ShouldUseTick() const;

	void SetCompassOwner(UCompassComponent* NewOwner);

	float GetDistanceFromTarget() const;

	bool UseDynamicScale() const;
};
