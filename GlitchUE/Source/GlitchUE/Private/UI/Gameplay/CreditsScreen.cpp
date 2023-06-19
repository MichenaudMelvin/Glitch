// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/CreditsScreen.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayerController.h"

UCreditsScreen::UCreditsScreen(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
	MainMenu = FSoftObjectPath("/Game/Maps/Main_Menu");
}

void UCreditsScreen::NativeOnInitialized(){
	Super::NativeOnInitialized();

	GoToCreditButton->OnClicked.AddDynamic(this, &UCreditsScreen::SwitchToCredits);
	BackToMenuButton->OnClicked.AddDynamic(this, &UCreditsScreen::BackToMainMenu);

	AddWidgetToFocusList(GoToCreditButton);
	AddWidgetToFocusList(BackToMenuButton);
}

void UCreditsScreen::NativeConstruct(){
	Super::NativeConstruct();

	CurrentController->ShowMouseCursor(true, this);
	Cast<AMainPlayerController>(CurrentController)->UnbindAll(true);
	UGameplayStatics::SetGamePaused(CurrentController, true);

	const EUMGSequencePlayMode::Type PlayMode = bFadeIn ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse;

	PlayAnimation(FadeAnimation, 0, 1, PlayMode, FadeTime);
}

void UCreditsScreen::SwitchToCredits(){
	MessageLayer->SetVisibility(ESlateVisibility::Hidden);
	CreditLayer->SetVisibility(ESlateVisibility::Visible);

	if(CurrentController->IsUsingGamepad()){
		BackToMenuButton->SetKeyboardFocus();
		Refocus();
	}
}

void UCreditsScreen::BackToMainMenu(){
	UGameplayStatics::OpenLevelBySoftObjectPtr(CurrentController, MainMenu, true, "SkipIntro");
}

void UCreditsScreen::SetEndMessage(const FString NewEndMessage){
	EndMessage->SetText(FText::FromString(NewEndMessage));

	AddToViewport();
}
