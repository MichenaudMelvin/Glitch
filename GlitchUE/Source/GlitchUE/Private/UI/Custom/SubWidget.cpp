// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Custom/SubWidget.h"
#include "Player/AbstractPlayerController.h"

void USubWidget::NativeOnInitialized(){
	Super::NativeOnInitialized();

	BackButton->OnClicked.AddDynamic(this, &USubWidget::BackToOwnerWidget);

	AddWidgetToFocusList(BackButton);
}

void USubWidget::NativeConstruct(){
	Super::NativeConstruct();

	CurrentController->OnPause.Clear();
	CurrentController->OnPause.AddDynamic(this, &USubWidget::BackToOwnerWidget);
}

void USubWidget::AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder){
	Super::AddToScreen(LocalPlayer, ZOrder);

	OwnerWidget->RemoveFromParent();
}

void USubWidget::BackToOwnerWidget(){
	CurrentController->OnPause.RemoveDynamic(this, &USubWidget::BackToOwnerWidget);

	RemoveFromParent();
	OwnerWidget->AddToViewport();
}

void USubWidget::SetOwnerWidget(UUserWidget* NewOwnerWidget){
	OwnerWidget = NewOwnerWidget;
}
