// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActivableComponent.generated.h"

#pragma region Delagates

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnActivated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnDesactivated);

#pragma endregion


UENUM(BlueprintType)
enum class EState : uint8 {

	Activated,

	Desactivated,
};

UCLASS(ClassGroup = "Common", meta=(BlueprintSpawnableComponent))
class GLITCHUE_API UActivableComponent : public UActorComponent{
	GENERATED_BODY()

public:	
	UActivableComponent();

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Activable")
	EState State;

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates")
	FKOnActivated OnActivated;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates")
	FKOnDesactivated OnDesactivated;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Activable")
	EState GetState();

	/**
	 * @brief Use it for activate the owner // Activate() is from UE
	 */
	UFUNCTION(BlueprintCallable, Category = "Activable")
	void ActivateObject();

	/**
	 * @brief Use it for desactivate the owner
	 */
	UFUNCTION(BlueprintCallable, Category = "Activable")
	void DesactivateObject();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsActivated() const;
};
