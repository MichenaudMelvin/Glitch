// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlacableActorData.h"
#include "PlacableActor.generated.h"

USTRUCT(BlueprintType)
struct FPlacableActorCreation{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<APlacableActor> Class;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UPlacableActorData* Data;
};

UCLASS()
class GLITCHUE_API APlacableActor : public AActor{
	GENERATED_BODY()
	
public:	
	APlacableActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	UPlacableActorData* CurrentData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Name")
	FName Name;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PlayerActions")
	void SetData(UPlacableActorData* NewData);
	virtual void SetData_Implementation(UPlacableActorData* NewData);

	UFUNCTION(BlueprintCallable, Category = "PlayerActions")
	void Upgrade();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,  Category = "Glitch")
	void GlitchUpgrade();
	virtual void GlitchUpgrade_Implementation();
};
