// Fill out your copyright notice in the Description page of Project Settings.

#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "Kismet/KismetSystemLibrary.h"

void UUsefullFunctions::OutlineComponent(bool SetOutline, UPrimitiveComponent* Component){
	if (!IsValid(Component)){
		return;
	}

	Component->SetRenderCustomDepth(SetOutline);
	Component->SetCustomDepthStencilValue(SetOutline ? 2 : 0);
}

bool UUsefullFunctions::CanSee(AActor* SelfActor, FVector StartLocation, AActor* ActorToSee, ECollisionChannel CollisionChannel){
	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;

	ActorsToIgnore.Add(SelfActor);

	UKismetSystemLibrary::LineTraceSingle(ActorToSee->GetWorld(), StartLocation, ActorToSee->GetActorLocation(), UEngineTypes::ConvertToTraceType(CollisionChannel), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, true, FLinearColor::Red, FLinearColor::Green, 0.1f);

	if (Hit.GetActor() == ActorToSee){
		return true;
	} else{
		return false;
	}
}

int UUsefullFunctions::ClampIntToArrayLength(const int IntToClamp, const int ArrayLength){
	if(IntToClamp < 0 ){
		return ArrayLength - 1;
	}

	if(IntToClamp > (ArrayLength - 1)){
		return 0;
	}

	return IntToClamp;
}

TArray<AActor*> UUsefullFunctions::SortActorsByDistanceToActor(TArray<AActor*> Actors, AActor* Target){
	QuickSortByDistance(Actors, 0, Actors.Num() - 1, Target);
	return Actors;
}

void UUsefullFunctions::QuickSortByDistance(TArray<AActor*>& InArray, const int Low, const int High, const AActor* Actor){
	if (InArray.Num() == 0 ) {
		UE_LOG(LogTemp, Warning, TEXT("Array null"));
		return;
	}
	
	int I = Low;
	int J = High;
	// Select a pivot
	const double Pivot = FVector::DistSquared(InArray[J]->GetActorLocation(), Actor->GetActorLocation());

	while (I <= J){

		while (FVector::DistSquared(InArray[I]->GetActorLocation(), Actor->GetActorLocation()) < Pivot){
			I++;
		}

		while (FVector::DistSquared(InArray[J]->GetActorLocation(), Actor->GetActorLocation()) > Pivot){
			J--;
		}

		if (I <= J){
			InArray.SwapMemory(I++, J--);
		}
	}

	if (J > Low){
		QuickSortByDistance(InArray, Low, J, Actor);
	}

	if (I < High){
		QuickSortByDistance(InArray, I, High, Actor);
	}
}