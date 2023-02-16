// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

#pragma region Delegates

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FKOnInteract, AMainPlayerController*, MainPlayerController, AMainPlayer*, MainPlayer);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnFeedback);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnUnfeedback);

#pragma endregion

UCLASS( ClassGroup=(Interactable), meta=(BlueprintSpawnableComponent) )
class GLITCHUE_API UInteractableComponent : public UActorComponent{
	GENERATED_BODY()

public:	
	UInteractableComponent();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interactable")
	bool CheckComponent(UPrimitiveComponent* ComponentToCheck);

protected:
	virtual void BeginPlay() override;

	TSet<UPrimitiveComponent*> InteractableComponentSet;

	#pragma region Add/Remove Interactable
public:
	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void AddInteractable(UPrimitiveComponent* ComponentToAdd);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void AddInteractables(TArray<UPrimitiveComponent*> ComponentsToAdd);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void RemoveInteractable(UPrimitiveComponent* ComponentToRemove);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void RemoveInteractables(TArray<UPrimitiveComponent*> ComponentsToRemove);

	#pragma endregion

	#pragma region Interaction & feedback
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Feedback")
	bool bOutlineFeedback = true;

	UFUNCTION(Category = "Feedback")
	void OutlineFeedback(const bool bOutline) const;

public:
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FKOnInteract OnInteract;

	UPROPERTY(BlueprintAssignable, Category = "Feedback")
	FKOnFeedback OnFeedback;

	UPROPERTY(BlueprintAssignable, Category = "Feedback")
	FKOnUnfeedback OnUnfeedback;

public:
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer);

	UFUNCTION(BlueprintCallable, Category = "Feedback")
	void Feedback();

	UFUNCTION(BlueprintCallable, Category = "Feedback")
	void Unfeedback();

	#pragma endregion

};
