// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UsefullFunctions.generated.h"

UCLASS()
class GLITCHUE_API UUsefullFunctions : public UBlueprintFunctionLibrary{
	GENERATED_BODY()
	
	public:
		UFUNCTION(BlueprintCallable)
		static void OutlineComponent(bool SetOutline, UPrimitiveComponent* Component);

		UFUNCTION(BlueprintCallable, BlueprintPure)
		static bool CanSee(AActor* SelfActor, FVector StartLocation, AActor* ActorToSee, ECollisionChannel CollisionChannel);

		UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sorting")
		//https://dev.epicgames.com/community/snippets/xBY/unreal-engine-sort-actor-array-based-on-distance-from-target
		static TArray<AActor*> SortActorsByDistanceToActor(TArray<AActor*> Actors, AActor* Target);

		UFUNCTION()
		//https://dev.epicgames.com/community/snippets/xBY/unreal-engine-sort-actor-array-based-on-distance-from-target
		static void QuickSortByDistance(TArray<AActor*>& InArray, int low, int high, const AActor* Actor);
};
