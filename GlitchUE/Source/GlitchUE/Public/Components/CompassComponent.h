// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CompassIcon.h"
#include "Components/CompassPivotIcon.h"
#include "Components/ActorComponent.h"
#include "CompassComponent.generated.h"

UCLASS(ClassGroup= "Compass", meta=(BlueprintSpawnableComponent))
class GLITCHUE_API UCompassComponent : public UActorComponent{
	GENERATED_BODY()

public:
	UCompassComponent();

	virtual void DestroyComponent(bool bPromoteChildren = false) override;

protected:
	TArray<UCompassIcon*> CompassIcons;

	TArray<UCompassPivotIcon*> CompassPivotIcons;

	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	bool bUsesZAxis = true;

	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	float CompassRadius = 80;

	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	FVector OffsetIcon = FVector::ZeroVector;

public:
	void AddIconToOwnerList(UCompassIcon* IconToAdd);

	void RemoveIconToOwnerList(UCompassIcon* IconToRemove);

	bool UsesZAxis() const;

	float GetCompassRadius() const;

	void SetCompassOffset(const FVector NewOffset);
};
