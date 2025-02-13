// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mark/Mark.h"
#include "FakeMark.generated.h"

class AGlitchMark;

UCLASS()
class GLITCHUE_API AFakeMark : public AMark{
	GENERATED_BODY()

public:
	AFakeMark();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY()
	AGlitchMark* GlitchMark;

	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FVector TargetPosition;

public:
	virtual void PlaceMark() override;

	virtual void ResetMark() override;

	virtual void Launch(const FRotator StartRotation) override;

	void SetTargetPosition(const FVector NewTargetPosition);

	void SetMarkCollision(const ECollisionResponse CollisionResponse) const;
};
