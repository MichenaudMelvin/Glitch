// Fill out your copyright notice in the Description page of Project Settings.


#include "Helpers/FunctionsLibrary/UsefullFunctions.h"

void UUsefullFunctions::OutlineComponent(bool SetOutline, UPrimitiveComponent* Component){
	if (!IsValid(Component)){
		return;
	}

	Component->SetRenderCustomDepth(SetOutline);
	Component->SetCustomDepthStencilValue(SetOutline ? 2 : 0);
}

TArray<AActor*> UUsefullFunctions::SortActorsByDistanceToActor(TArray<AActor*> Actors, AActor* Target){
	QuickSortByDistance(Actors, 0, Actors.Num() - 1, Target);
	return Actors;
}

void UUsefullFunctions::QuickSortByDistance(TArray<AActor*>& InArray, int low, int high, const AActor* Actor){
	int i = low;
	int j = high;
	// Select a pivot
	double pivot = FVector::DistSquared(InArray[j]->GetActorLocation(), Actor->GetActorLocation());

	while (i <= j){

		while (FVector::DistSquared(InArray[i]->GetActorLocation(), Actor->GetActorLocation()) < pivot){
			i++;
		}

		while (FVector::DistSquared(InArray[j]->GetActorLocation(), Actor->GetActorLocation()) > pivot){
			j--;
		}

		if (i <= j){
			InArray.SwapMemory(i++, j--);
		}
	}

	if (j > low){
		QuickSortByDistance(InArray, low, j, Actor);
	}

	if (i < high){
		QuickSortByDistance(InArray, i, high, Actor);
	}
}