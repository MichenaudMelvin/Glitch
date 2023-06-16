// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/PopUpImage.h"

#include "Kismet/GameplayStatics.h"

void UPopUpImage::NativeOnInitialized(){
	Super::NativeOnInitialized();

	BackButton->OnClicked.AddDynamic(this, &UPopUpImage::RemoveFromParent);

	AddWidgetToFocusList(BackButton);
}

void UPopUpImage::NativeConstruct(){
	Super::NativeConstruct();

	CurrentController->ShowMouseCursor(true, this);
	UGameplayStatics::SetGamePaused(this, true);
}

void UPopUpImage::NativeDestruct(){
	Super::NativeDestruct();

	CurrentController->ShowMouseCursor(false, nullptr);
	UGameplayStatics::SetGamePaused(this, false);
}

void UPopUpImage::ShowPopUpImage(UObject* Image){
	AddToViewport();

	PopUpIMG->SetBrushResourceObject(Image);
}
