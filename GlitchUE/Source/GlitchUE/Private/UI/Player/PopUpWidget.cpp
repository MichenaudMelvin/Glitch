// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PopUpWidget.h"
#include "Player/MainPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UPopUpWidget::NativeOnInitialized(){
	Super::NativeOnInitialized();

	MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	BackButton->OnClicked.AddDynamic(this, &UPopUpWidget::RemoveFromParent);
}

void UPopUpWidget::NativeDestruct(){
	Super::NativeDestruct();

	MainPlayerController->SelectNewGameplayMode(MainPlayerController->GetGameplayMode());
	MainPlayerController->bShowMouseCursor = false;

	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void UPopUpWidget::ShowPopUp(const FString PopUpMessage){
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	MainPlayerController->UnbindAll();
	MainPlayerController->bShowMouseCursor = true;

	PopUpText->SetText(FText::FromString(PopUpMessage));

	AddToViewport();
}
