// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/Settings/SettingsContainerMenu.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/Menu/Settings/ButtonCategory.h"
#include "UI/Menu/Settings/ButtonCategoryData.h"

void USettingsContainerMenu::NativeOnInitialized(){
	bIsDynamicFocusList = true;

	Super::NativeOnInitialized();

	for(int i = 0; i < SettingsWidgetsClasses.Num(); i++){
		USettingsMenu* CurrentWidget = Cast<USettingsMenu>(CreateWidget(this, SettingsWidgetsClasses[i]));

		AllSettingsWidgets.Add(CurrentWidget);
		CurrentWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		UButtonCategoryData* CurrentData = NewObject<UButtonCategoryData>();

		CurrentData->SettingsContainer = this;
		CurrentData->WidgetToShow = CurrentWidget;
		CurrentData->Index = i;
		CurrentData->Category = CurrentWidget->GetSettingCategory();

		SettingsButtonCategories->AddItem(CurrentData);
	}
}

void USettingsContainerMenu::NativeConstruct(){
	Super::NativeConstruct();

	CurrentController->OnGamepadTriggers.AddDynamic(this, &USettingsContainerMenu::HorizontalScroll);
	CurrentController->OnSwitchToGamepad.AddDynamic(this, &USettingsContainerMenu::ShowGamepadInformation);
	CurrentController->OnSwitchToKeyboard.AddDynamic(this, &USettingsContainerMenu::HideGamepadInformation);

	CurrentController->IsUsingGamepad() ? ShowGamepadInformation() : HideGamepadInformation();

	AddWidgetToFocusList(ApplySettingsButton);
	AddWidgetToFocusList(ResetToDefaultButton);
	AddWidgetToFocusList(BackButton);

	SetCurrentShownWidget(AllSettingsWidgets[0]);
	HorizontalIndex = 0;
}

void USettingsContainerMenu::NativeDestruct(){
	Super::NativeDestruct();

	CurrentController->OnGamepadTriggers.RemoveDynamic(this, &USettingsContainerMenu::HorizontalScroll);
	CurrentController->OnSwitchToGamepad.RemoveDynamic(this, &USettingsContainerMenu::ShowGamepadInformation);
	CurrentController->OnSwitchToKeyboard.RemoveDynamic(this, &USettingsContainerMenu::HideGamepadInformation);
}

void USettingsContainerMenu::BindButtons(){
	ApplySettingsButton->OnClicked.Clear();
	ResetToDefaultButton->OnClicked.Clear();

	ApplySettingsButton->OnClicked.AddDynamic(CurrentShownWidget, &USettingsMenu::UpdateSettings);
	ResetToDefaultButton->OnClicked.AddDynamic(CurrentShownWidget, &USettingsMenu::ResetSettings);
}

void USettingsContainerMenu::HorizontalScroll(float AxisValue){
	if(AxisValue == 0){
		return;
	}

	HorizontalIndex = FMath::Clamp(HorizontalIndex += AxisValue, 0, SettingsButtonCategories->GetDisplayedEntryWidgets().Num() - 1);
	Cast<UButtonCategory>(SettingsButtonCategories->GetDisplayedEntryWidgets()[HorizontalIndex])->SelectCategory();
}

void USettingsContainerMenu::ShowGamepadInformation(){
	LeftScrollInformation->SetVisibility(ESlateVisibility::Visible);
	RightScrollInformation->SetVisibility(ESlateVisibility::Visible);
}

void USettingsContainerMenu::HideGamepadInformation(){
	LeftScrollInformation->SetVisibility(ESlateVisibility::Hidden);
	RightScrollInformation->SetVisibility(ESlateVisibility::Hidden);
}

void USettingsContainerMenu::FocusHorizontalIndex(){
	for(int i = 0; i < SettingsButtonCategories->GetDisplayedEntryWidgets().Num(); i++){
		Cast<UButtonCategory>(SettingsButtonCategories->GetDisplayedEntryWidgets()[i])->GetButton()->UnReceiveFocus();
	}

	if(SettingsButtonCategories->GetDisplayedEntryWidgets().IsValidIndex(HorizontalIndex)){
		Cast<UButtonCategory>(SettingsButtonCategories->GetDisplayedEntryWidgets()[HorizontalIndex])->GetButton()->ReceiveFocus();
	}
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

	FocusHorizontalIndex();

	AddWidgetsToFocusList(CurrentShownWidget->GetFocusableWidgets());

	if(CurrentController->IsUsingGamepad()){
		LastFocusWidgetIndex = 0;
		Refocus();
	}

	

	SettingsWidgetContainer->AddChild(CurrentShownWidget);

	UCanvasPanelSlot* CurrentSlot = Cast<UCanvasPanelSlot>(CurrentShownWidget->Slot);

	CurrentSlot->SetAnchors(FullSizeAnchors);
	CurrentSlot->SetOffsets(FullSizeMargin);
}

void USettingsContainerMenu::SetHorizontalIndex(int NewIndex){
	HorizontalIndex = NewIndex;
}
