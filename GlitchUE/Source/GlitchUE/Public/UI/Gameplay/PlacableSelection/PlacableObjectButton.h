// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Gameplay/PlacableSelection/SelectionButton.h"
#include "PlacableObjectButton.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UPlacableObjectButton : public USelectionButton{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(EditAnywhere)
	UPlacableActorData* Data;

public:
	virtual void Select() override;

	bool CompareData(const UPlacableActorData* DataToCompare) const;
};
