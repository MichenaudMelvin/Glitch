// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/Settings/ButtonCategory.h"

void UButtonCategory::NativeOnInitialized(){
	Super::NativeOnInitialized();

	Button->OnClicked.AddDynamic(this, &UButtonCategory::SelectCategory);
}

void UButtonCategory::NativeOnListItemObjectSet(UObject* ListItemObject){
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	CurrentData = Cast<UButtonCategoryData>(ListItemObject);

	SettingsContainer = CurrentData->SettingsContainer;
	WidgetToShow = CurrentData->WidgetToShow;
	CategoryText->SetText(CurrentData->Category);
}

void UButtonCategory::SelectCategory(){
	SettingsContainer->SetCurrentShownWidget(WidgetToShow);
}
