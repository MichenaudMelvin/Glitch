// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

#pragma region Delegates

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnReciveDamages);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnHealthNull);

#pragma endregion

UCLASS( ClassGroup=(Health), meta=(BlueprintSpawnableComponent) )
class GLITCHUE_API UHealthComponent : public UActorComponent{
	GENERATED_BODY()

public:	
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	bool bCanTakeDamages = true;

	UFUNCTION(BlueprintCallable, Category = "Damages")
	void TakeDamages(float DamagesAmount);

	UFUNCTION(BlueprintCallable, Category = "Damages")
	void TakeMaxDamages();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float HealthAmount);

public:
	UPROPERTY(BlueprintAssignable, Category = "Damages")
	FKOnReciveDamages OnReciveDamages;

	UPROPERTY(BlueprintAssignable, Category = "Damages")
	FKOnHealthNull OnHealthNull;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Damages")
	bool GetCanTakeDamages();

	UFUNCTION(BlueprintCallable, Category = "Damages")
	void SetCanTakeDamages(bool bValue);
};
