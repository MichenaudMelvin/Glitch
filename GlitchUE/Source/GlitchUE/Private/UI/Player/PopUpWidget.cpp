// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PopUpWidget.h"
#include "Player/MainPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UPopUpWidget::NativeOnInitialized(){
	Super::NativeOnInitialized();

	MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void UPopUpWidget::NativeDestruct(){
	Super::NativeDestruct();

	MainPlayerController->SelectNewGameplayMode(MainPlayerController->GetGameplayMode());
	MainPlayerController->ShowMouseCursor(false);

	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void UPopUpWidget::InitWidget(){
	BackButton->OnClicked.Clear();

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	MainPlayerController->UnbindAll();
	MainPlayerController->ShowMouseCursor(true);
	AddToViewport();
}

void UPopUpWidget::GoToNextPopUp(){

	PopUpText->SetText(FText::FromString(CurrentPopUpArray[ArrayCount]));

	ArrayCount++;

	if(ArrayCount == CurrentPopUpArray.Num()){
		BackButton->OnClicked.Clear();
		BackButton->OnClicked.AddDynamic(this, &UPopUpWidget::RemoveFromParent);
	}
}

void UPopUpWidget::ShowPopUp(const FString PopUpMessage){
	InitWidget();

	BackButton->OnClicked.AddDynamic(this, &UPopUpWidget::RemoveFromParent);

	PopUpText->SetText(FText::FromString(PopUpMessage));
}

void UPopUpWidget::ShowMultiplePopUps(const TArray<FString> PopUpMessages){
	ArrayCount = 0;
	CurrentPopUpArray = PopUpMessages;

	InitWidget();

	BackButton->OnClicked.AddDynamic(this, &UPopUpWidget::GoToNextPopUp);
	GoToNextPopUp();
}
