// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/AbstractPlayerController.h"
#include "CustomUserWidget.generated.h"

UCLASS(Abstract)
class GLITCHUE_API UCustomUserWidget : public UUserWidget{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void FocusWidgets();

#if WITH_EDITOR
	bool CheckValidity(const UWidget* WidgetToCheck) const;
#endif

	UFUNCTION()
	void UnFocusAll();

	UFUNCTION(BlueprintCallable, Category = "Focus")
	void Refocus();

	UPROPERTY(BlueprintReadOnly, Category = "Controller")
	AAbstractPlayerController* CurrentController;

	UPROPERTY()
	TArray<UWidget*> FocusList;

	int LastFocusWidgetIndex = 0;

	/**
	 * @brief true mean it will clear the focust list at DestructWidget
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Focus")
	bool bIsDynamicFocusList = false;

	UPROPERTY(EditDefaultsOnly, Category = "Focus")
	bool bIsFocusNeeded = true;

public:
	UFUNCTION(BlueprintCallable, Category = "Focus")
	void AddWidgetToFocusList(UWidget* WidgetToAdd);

	UFUNCTION(BlueprintCallable, Category = "Focus")
	void AddWidgetsToFocusList(TArray<UWidget*> WidgetsToAdd);

	UFUNCTION(BlueprintCallable, Category = "Focus")
	void RemoveWidgetToFocusList(UWidget* WidgetToRemove);

	UFUNCTION(BlueprintCallable, Category = "Focus")
	void RemoveWidgetsToFocusList(TArray<UWidget*> WidgetsToRemove);

	UFUNCTION(BlueprintCallable, Category = "Focus")
	TArray<UWidget*> GetFocusList() const;

	void ResetLastFocusWidgetIndex();
};
