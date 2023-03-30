// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ListView.h"
#include "Player/UI/SubWidget.h"
#include "SaveSelection.generated.h"

class USaveAction;
class USaveButton;

UCLASS()
class GLITCHUE_API USaveSelection : public USubWidget{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UListView* SaveListView;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USaveAction* SaveAction;

	UPROPERTY(EditDefaultsOnly)
	int WorldSaveSlot = 5;

public:
	void SetupSaveActions(USaveButton* SaveButton) const;

	UFUNCTION(BlueprintCallable)
	void CreateEntry(UObject* Item, UUserWidget* Widget);

	UFUNCTION(BlueprintCallable)
	void OpenSaveSelection(const bool bAllowSave = false);
};
