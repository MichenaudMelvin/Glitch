// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Gameplay/PlacableSelection/SelectionButton.h"
#include "PlacableObjectButton.generated.h"

class UWheel;

UCLASS(Abstract)
class GLITCHUE_API UPlacableObjectButton : public USelectionButton{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(EditAnywhere)
	UPlacableActorData* Data;

	UWheel* Wheel;

	UFUNCTION()
	void OnClick();

public:
	virtual void Select() override;

	virtual void UnSelect() override;

	virtual void BindButtons() override;

	virtual void UnbindButtons() override;

	bool CompareData(const UPlacableActorData* DataToCompare) const;

	void SetWheel(UWheel* NewWheel);
};
