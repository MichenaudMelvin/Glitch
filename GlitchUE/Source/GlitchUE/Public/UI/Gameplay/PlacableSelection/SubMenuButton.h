// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Gameplay/PlacableSelection/SelectionButton.h"
#include "SubMenuButton.generated.h"

UCLASS(Abstract)
class GLITCHUE_API USubMenuButton : public USelectionButton{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	TArray<USelectionButton*> AffectedButtons;

	UPROPERTY(EditAnywhere)
	FText ButtonName;

public:
	virtual void Select() override;

	virtual void UnSelect() override;

	void SetAffectedButtons(const TArray<USelectionButton*> NewButtons);
};
