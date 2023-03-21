// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/SubWidget.h"

void USubWidget::NativeOnInitialized(){
	Super::NativeConstruct();

	BackButton->OnClicked.AddDynamic(this, &USubWidget::BackToOwnerWidget);
}

void USubWidget::AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder){
	Super::AddToScreen(LocalPlayer, ZOrder);

	OwnerWidget->RemoveFromParent();
}

void USubWidget::BackToOwnerWidget(){
	RemoveFromParent();
	OwnerWidget->AddToViewport();
}
