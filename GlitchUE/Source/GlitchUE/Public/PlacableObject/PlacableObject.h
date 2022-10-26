// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlacableObject.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom))
class GLITCHUE_API UPlacableObject : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlacableObject();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FVector OriginalLocation;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// <summary>
	/// 
	/// </summary>
	/// <returns>True if the object can be place</returns>
	bool PreviewObject();
	
	void PlaceObject(FVector NewLocation);

	FVector GetOriginalLocation();

	UFUNCTION(BlueprintCallable)
	void ResetObject();
};
