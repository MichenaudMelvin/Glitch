// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "Components/ActorComponent.h"
#include "CompassIcon.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLITCHUE_API UCompassIcon : public UActorComponent{
	GENERATED_BODY()

public:	
	UCompassIcon();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UPaperSprite* OwnerSprite;

	FVector OwnerLocation;

public:
	UPaperSprite* GetOwnerSprite();

	FVector GetOwnerLocation();

};
