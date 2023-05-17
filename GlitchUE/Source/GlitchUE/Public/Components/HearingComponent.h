// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ActorComponent.h"
#include "HearingComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLITCHUE_API UHearingComponent : public UActorComponent{
	GENERATED_BODY()

public:
	UHearingComponent();

protected:
	virtual void BeginPlay() override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	void HearSound() const;

	UBlackboardComponent* BlackboardOwner;

	UPROPERTY(EditDefaultsOnly, Category = "Hearing")
	float MaxSoundTolerance = 1;

	FVector CurrentSoundLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	TArray<TSubclassOf<AActor>> ClassToIgnore;

	bool IsClassValid(const AActor* ActorToCheck) const;

public:
	void RegisterSound(AActor* Investigator, const FVector SoundLocation);
};
