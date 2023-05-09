// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CompassIcon.h"
#include "Components/CompassPivotIcon.h"
#include "Components/ActorComponent.h"
#include "CompassComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLITCHUE_API UCompassComponent : public UActorComponent{
	GENERATED_BODY()

public:
	UCompassComponent();

protected:
	TArray<UCompassIcon*> CompassIcons;

	TArray<UCompassPivotIcon*> CompassPivotIcons;

	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	bool bUsesZAxis = true;

public:
	void AddIconToOwnerList(UCompassIcon* IconToAdd);

	void RemoveIconToOwnerList(UCompassIcon* IconToRemove);

	bool UsesZAxis() const;
};
