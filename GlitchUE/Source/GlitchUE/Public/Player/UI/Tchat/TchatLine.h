// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TchatLineData.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "TchatLine.generated.h"

UCLASS()
class GLITCHUE_API UTchatLine : public UUserWidget, public IUserObjectListEntry{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UTchatLineData* CurrentData;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* LocationText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* SpeakerText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MessageText;
};
