// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/Settings/SettingsContainerMenu.h"
#include "Components/CanvasPanelSlot.h"
#include "Player/UI/Settings/ButtonCategoryData.h"

void USettingsContainerMenu::NativeOnInitialized(){
	Super::NativeOnInitialized();

	for(int i = 0; i < SettingsWidgetsClasses.Num(); i++){
		USettingsMenu* CurrentWidget = Cast<USettingsMenu>(CreateWidget(this, SettingsWidgetsClasses[i]));

		AllSettingsWidgets.Add(CurrentWidget);

		UButtonCategoryData* CurrentData = NewObject<UButtonCategoryData>();

		CurrentData->SettingsContainer = this;
		CurrentData->WidgetToShow = CurrentWidget;
		CurrentData->Category = CurrentWidget->GetSettingCategory();

		SettingsButtonCategories->AddItem(CurrentData);
	}
}

void USettingsContainerMenu::NativeConstruct(){
	Super::NativeConstruct();

	SetCurrentShownWidget(AllSettingsWidgets[0]);

	for(int i = 1; i < AllSettingsWidgets.Num(); i++){
		AllSettingsWidgets[i]->RemoveFromParent();
	}
}

void USettingsContainerMenu::SetCurrentShownWidget(UUserWidget* NewWidget){
	if(CurrentShownWidget == NewWidget){
		return;
	}

	if(IsValid(CurrentShownWidget)){
		CurrentShownWidget->RemoveFromParent();
	}

	CurrentShownWidget = NewWidget;
	CurrentShownWidget->AddToViewport();

	SettingsWidgetContainer->AddChild(CurrentShownWidget);

	UCanvasPanelSlot* CurrentSlot = Cast<UCanvasPanelSlot>(CurrentShownWidget->Slot);

	CurrentSlot->SetAnchors(FullSizeAnchors);
	CurrentSlot->SetOffsets(FullSizeMargin);
}