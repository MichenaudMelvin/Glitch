// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CompassIcon.h"
#include "Components/SceneComponent.h"
#include "CompassPivotIcon.generated.h"

class UCompassComponent;

UCLASS(ClassGroup= "Compass", meta=(BlueprintSpawnableComponent))
class GLITCHUE_API UCompassPivotIcon : public USceneComponent{
	GENERATED_BODY()

public:
	UCompassPivotIcon();

protected:
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	UStaticMeshComponent* ChildMesh;

	UCompassComponent* Compass;

	AActor* TargetToLookAt;

	UCompassIcon* CurrentCompassIcon;

	void SelectRotation();

	void SelectScale() const;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitPivotIcon(UCompassComponent* CompassComp, UCompassIcon* TargetCompassIcon);
};
