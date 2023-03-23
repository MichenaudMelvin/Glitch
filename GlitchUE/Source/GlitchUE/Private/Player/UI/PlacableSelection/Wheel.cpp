// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/PlacableSelection/Wheel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Player/MainPlayerController.h"

void UWheel::NativeOnInitialized(){
	Super::NativeOnInitialized();

	TArray<USelectionButton*> TurretsList;
	TurretsList.Add(PlacableButton1);
	TurretsList.Add(PlacableButton2);
	TurretsList.Add(PlacableButton3);
	TurretsList.Add(PlacableButton4);
	
	TurretSubMenuButton->SetAffectedButtons(TurretsList);
	
	TArray<USelectionButton*> TrapsList;
	TrapsList.Add(PlacableButton5);
	TrapsList.Add(PlacableButton6);
	TrapsList.Add(PlacableButton7);
	TrapsList.Add(PlacableButton8);
	
	TrapSubMenuButton->SetAffectedButtons(TrapsList);

	MainButtonList.Add(NormalModeButton);
	MainButtonList.Add(TurretSubMenuButton);
	MainButtonList.Add(TrapSubMenuButton);
	MainButtonList.Add(DestructionModeButton);

	SelectedButton = NormalModeButton;
}

void UWheel::NativeConstruct(){
	Super::NativeConstruct();

	const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld())/2;
	
	MainPlayerController->SetMouseLocation(ViewportSize.X, ViewportSize.Y);

	for(int i = 0; i < ButtonList.Num(); i++){
		ButtonList[i]->UnSelect();
	}
}

void UWheel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime){
	Super::NativeTick(MyGeometry, InDeltaTime);

	RadialSelection();
}

void UWheel::RadialSelection(){
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());

	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Thing to print %s"), *ViewportSize.ToString()));

	// à teseter avec un plus grand ecran le /2 mais ça a l'air bon
	MousePosition -= ViewportSize.X >= 1920 || ViewportSize.Y >= 1080 ? ViewportSize/2 : ViewportSize;

	float MaxPos = 300000;

	USelectionButton* OldSelected = SelectedButton;

	for(int i = 0; i < MainButtonList.Num(); i++){

		FVector2D SlotPosition = Cast<UCanvasPanelSlot>(MainButtonList[i]->Slot)->GetPosition();
		const float SlotDistance = SlotPosition.Distance(SlotPosition, MousePosition);

		if(SlotDistance < MaxPos){
			SelectedButton = MainButtonList[i];
			MaxPos = SlotDistance;
		}
	}

	if(OldSelected != SelectedButton){
		OldSelected->UnSelect();
		SelectedButton->Select();
	}
}
