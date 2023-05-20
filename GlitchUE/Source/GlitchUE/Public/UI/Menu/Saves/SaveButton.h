// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveSelection.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Saves/WorldSave.h"
#include "SaveButton.generated.h"

UCLASS(Abstract)
class GLITCHUE_API USaveButton : public UUserWidget, public IUserObjectListEntry{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	USaveSelection* SaveSelectionOwner;

	AGameModeBase* GameMode;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* SaveButton;

	UWorldSave* CurrentSave;

	int Index;

	UFUNCTION()
	void OnSelectSave();

public:
	void SetIndex(const int NewIndex);

	void SetOwner(USaveSelection* NewOwner);

	UFUNCTION()
	void Save();

	UFUNCTION()
	void Load();
};
