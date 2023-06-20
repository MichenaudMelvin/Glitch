// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/Settings/SettingsContainerMenu.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/Menu/Settings/ButtonCategoryData.h"

void USettingsContainerMenu::NativeOnInitialized(){
	Super::NativeOnInitialized();

	for(int i = 0; i < SettingsWidgetsClasses.Num(); i++){
		USettingsMenu* CurrentWidget = Cast<USettingsMenu>(CreateWidget(this, SettingsWidgetsClasses[i]));

		AllSettingsWidgets.Add(CurrentWidget);
		CurrentWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		UButtonCategoryData* CurrentData = NewObject<UButtonCategoryData>();

		CurrentData->SettingsContainer = this;
		CurrentData->WidgetToShow = CurrentWidget;
		CurrentData->Category = CurrentWidget->GetSettingCategory();

		SettingsButtonCategories->AddItem(CurrentData);
	}

	bIsDynamicFocusList = true;
}

void USettingsContainerMenu::NativeConstruct(){
	Super::NativeConstruct();

	SetCurrentShownWidget(AllSettingsWidgets[0]);

	AddWidgetToFocusList(ApplySettingsButton);
	AddWidgetToFocusList(ResetToDefaultButton);
	AddWidgetToFocusList(BackButton);
}

void USettingsContainerMenu::BindButtons(){
	ApplySettingsButton->OnClicked.Clear();
	ResetToDefaultButton->OnClicked.Clear();

	ApplySettingsButton->OnClicked.AddDynamic(CurrentShownWidget, &USettingsMenu::UpdateSettings);
	ResetToDefaultButton->OnClicked.AddDynamic(CurrentShownWidget, &USettingsMenu::ResetSettings);
}

void USettingsContainerMenu::SetCurrentShownWidget(USettingsMenu* NewWidget){
	if(CurrentShownWidget == NewWidget){
		AddWidgetsToFocusList(CurrentShownWidget->GetFocusableWidgets());
		return;
	}

	if(IsValid(CurrentShownWidget)){
		CurrentShownWidget->RemoveFromParent();
		RemoveWidgetsToFocusList(CurrentShownWidget->GetFocusableWidgets());
	}

	CurrentShownWidget = NewWidget;

	BindButtons();

	CurrentShownWidget->AddToViewport();

	AddWidgetsToFocusList(CurrentShownWidget->GetFocusableWidgets());

	SettingsWidgetContainer->AddChild(CurrentShownWidget);

	UCanvasPanelSlot* CurrentSlot = Cast<UCanvasPanelSlot>(CurrentShownWidget->Slot);

	CurrentSlot->SetAnchors(FullSizeAnchors);
	CurrentSlot->SetOffsets(FullSizeMargin);
}
