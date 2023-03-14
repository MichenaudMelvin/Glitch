// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UsefullFunctions.generated.h"

UCLASS()
class GLITCHUE_API UUsefullFunctions : public UBlueprintFunctionLibrary{
	GENERATED_BODY()

	public:
		UFUNCTION(BlueprintCallable, Category = "Outline")
		static void OutlineComponent(bool SetOutline, UPrimitiveComponent* Component);

		UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Outline")
		static bool CanSee(AActor* SelfActor, FVector StartLocation, AActor* ActorToSee, ECollisionChannel CollisionChannel);

		UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math")
		static int ClampIntToArrayLength(const int IntToClamp, const int ArrayLength);

		UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math")
		static int ClampZeroOne(const int IntToClamp);

		UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math")
		static float ClampZeroOneFloat(const float FloatToClamp);

		UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math")
		static FVector AddToVector(FVector Vector, const float X, const float Y, const float Z);

		UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math")
		static FRotator AddToRotator(FRotator Rotator, const float X, const float Y, const float Z);

		UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sorting")
		//https://dev.epicgames.com/community/snippets/xBY/unreal-engine-sort-actor-array-based-on-distance-from-target
		static TArray<AActor*> SortActorsByDistanceToActor(TArray<AActor*> Actors, AActor* Target);

		UFUNCTION()
		//https://dev.epicgames.com/community/snippets/xBY/unreal-engine-sort-actor-array-based-on-distance-from-target
		static void QuickSortByDistance(TArray<AActor*>& InArray, const int Low, const int High, const AActor* Actor);

		UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DeterminesOutputType = "SaveClass"), Category = "Saves")
		static UAbstractSave* CreateSave(const TSubclassOf<UAbstractSave> SaveClass);

		UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "SaveObject"), Category = "Saves")
		static UAbstractSave* SaveToSlot(UAbstractSave* SaveObject, const int UserIndex);

		UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DeterminesOutputType = "SaveClass"), Category = "Saves")
		static UAbstractSave* LoadSave(const TSubclassOf<UAbstractSave> SaveClass, const int UserIndex, const bool bCreateNewSaveIfDoesntExist = true);

		UFUNCTION(BlueprintCallable, Category = "Saves")
		static bool DeleteSaveSlot(UAbstractSave* SaveObject, const int UserIndex);
};
