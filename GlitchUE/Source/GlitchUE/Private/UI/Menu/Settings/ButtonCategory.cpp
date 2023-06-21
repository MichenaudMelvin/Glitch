// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu//Settings/ButtonCategory.h"

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
	Index = CurrentData->Index;
}

void UButtonCategory::SelectCategory(){
	SettingsContainer->SetHorizontalIndex(Index);
	SettingsContainer->SetCurrentShownWidget(WidgetToShow);
}

UCustomButton* UButtonCategory::GetButton() const{
	return Button;
}
