// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/PopUpWidget.h"
#include "Player/MainPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UPopUpWidget::NativeOnInitialized(){
	Super::NativeOnInitialized();

	MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AddWidgetToFocusList(BackButton);
}

void UPopUpWidget::NativeDestruct(){
	Super::NativeDestruct();

	UGameplayStatics::SetGamePaused(GetWorld(), false);

#if WITH_EDITOR
	if(!IsValid(MainPlayerController)){
		return;
	}
#endif

	MainPlayerController->ShowMouseCursor(false, nullptr);

	if(bRebindAtDestruct){
		MainPlayerController->BindNormalMode();
	}
}

void UPopUpWidget::InitWidget(){
	AddToViewport();

	BackButton->OnClicked.Clear();

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	MainPlayerController->UnbindAll();
	MainPlayerController->ShowMouseCursor(true, this);
}

void UPopUpWidget::GoToNextPopUp(){

	PopUpText->SetText(FText::FromString(CurrentPopUpArray[ArrayCount]));

	ArrayCount++;

	if(ArrayCount == CurrentPopUpArray.Num()){
		BackButton->OnClicked.Clear();
		BackButton->OnClicked.AddDynamic(this, &UPopUpWidget::RemoveFromParent);
	}
}

void UPopUpWidget::ShowPopUp(const FString PopUpMessage, const bool bRebindAtEnd){
	bRebindAtDestruct = bRebindAtEnd;
	InitWidget();

	BackButton->OnClicked.AddDynamic(this, &UPopUpWidget::RemoveFromParent);

	PopUpText->SetText(FText::FromString(PopUpMessage));
}

void UPopUpWidget::ShowMultiplePopUps(const TArray<FString> PopUpMessages, const bool bRebindAtEnd){
	bRebindAtDestruct = bRebindAtEnd;
	ArrayCount = 0;
	CurrentPopUpArray = PopUpMessages;

	InitWidget();

	BackButton->OnClicked.AddDynamic(this, &UPopUpWidget::GoToNextPopUp);
	GoToNextPopUp();
}
