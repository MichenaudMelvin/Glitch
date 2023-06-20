// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/PopUpImage.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayerController.h"

void UPopUpImage::NativeOnInitialized(){
	Super::NativeOnInitialized();

	BackButton->OnClicked.AddDynamic(this, &UPopUpImage::RemoveFromParent);

	AddWidgetToFocusList(BackButton);
}

void UPopUpImage::NativeConstruct(){
	Super::NativeConstruct();

	CurrentController->ShowMouseCursor(true, this);

	const AMainPlayerController* CastedController = Cast<AMainPlayerController>(CurrentController);

	if(CastedController->GetTchatWidget()->IsInViewport()){
		CastedController->GetTchatWidget()->RemoveFromParent();
	}

	if(CastedController->GetTimerWidget()->IsTimerRunning()){
		CastedController->GetTimerWidget()->RemoveFromParent();
	}

	CastedController->GetPlayerStatsWidget()->RemoveFromParent();

	UGameplayStatics::SetGamePaused(this, true);
}

void UPopUpImage::NativeDestruct(){
	Super::NativeDestruct();

	CurrentController->ShowMouseCursor(false, nullptr);

	const AMainPlayerController* CastedController = Cast<AMainPlayerController>(CurrentController);

	if(CastedController->GetTimerWidget()->IsTimerRunning()){
		CastedController->GetTimerWidget()->AddToViewport();
	}

	CastedController->GetPlayerStatsWidget()->AddToViewport();

	UGameplayStatics::SetGamePaused(this, false);
}
