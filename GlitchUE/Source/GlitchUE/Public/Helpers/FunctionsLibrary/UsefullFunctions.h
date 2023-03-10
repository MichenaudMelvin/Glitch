// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

		UFUNCTION(BlueprintCallable, BlueprintPure)
		static int ClampIntToArrayLength(const int IntToClamp, const int ArrayLength);

		UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sorting")
		//https://dev.epicgames.com/community/snippets/xBY/unreal-engine-sort-actor-array-based-on-distance-from-target
		static TArray<AActor*> SortActorsByDistanceToActor(TArray<AActor*> Actors, AActor* Target);

		UFUNCTION()
		//https://dev.epicgames.com/community/snippets/xBY/unreal-engine-sort-actor-array-based-on-distance-from-target
		static void QuickSortByDistance(TArray<AActor*>& InArray, const int Low, const int High, const AActor* Actor);

		UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType=""))
		static UAbstractSave* SaveToSlot(UAbstractSave* SaveObject, int UserIndex);

		UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DeterminesOutputType="SaveClass"))
		static UAbstractSave* LoadSave(TSubclassOf<UAbstractSave> SaveClass, int UserIndex);
};
