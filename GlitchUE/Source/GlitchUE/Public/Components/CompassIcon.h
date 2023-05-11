// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/MainPlayer.h"
#include "CompassIcon.generated.h"

class UCompassComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

	UPROPERTY(EditDefaultsOnly, Category = "Appearance")
	float TargetScale = 0.25f;

	UPROPERTY(EditDefaultsOnly, Category = "Class")
	TSubclassOf<AActor> TargetCompassOwnerClass = AMainPlayer::StaticClass();

	UCompassComponent* CompassOwner;

public:
	UStaticMesh* GetOwnerMesh() const;

	FVector GetOwnerLocation() const;

	bool CanBeDrawn() const;

	void SetAllowDraw(const bool bAllow);

	FVector GetTargetScale() const;
};
