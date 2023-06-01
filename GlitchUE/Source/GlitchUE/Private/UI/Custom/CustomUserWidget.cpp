// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Custom/CustomUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetInputLibrary.h"
#include "UI/UIFocus.h"

void UCustomUserWidget::NativeOnInitialized(){
	Super::NativeOnInitialized();

	CurrentController = Cast<AAbstractPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void UCustomUserWidget::NativeConstruct(){
	Super::NativeConstruct();

	CurrentController->OnAnyKey.AddDynamic(this, &UCustomUserWidget::OnAnyKey);
}

void UCustomUserWidget::NativeDestruct(){
	CurrentController->OnAnyKey.RemoveDynamic(this, &UCustomUserWidget::OnAnyKey);

	// if the last focused widget was remove from the focus list
	if(!FocusList.IsValidIndex(LastFocusWidgetIndex)){
		Super::NativeDestruct();
		return;
	}

	#if WITH_EDITOR
	if(!CheckValidity(FocusList[LastFocusWidgetIndex])){
		return;
	}
	#endif

	Cast<IUIFocus>(FocusList[LastFocusWidgetIndex])->UnReceiveFocus();

	Super::NativeDestruct();
}

void UCustomUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime){
	Super::NativeTick(MyGeometry, InDeltaTime);

	const FVector2D CurrentMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	if(bIsFocusNeeded && LastedMousePosition != CurrentMousePosition){
		LastedMousePosition = CurrentMousePosition;
		UnFocusAll();
	}

	FocusWidgets();
}

void UCustomUserWidget::FocusWidgets(){
	if(!bIsFocusNeeded){
		return;
	}

	for(int i = 0; i < FocusList.Num(); i++){

		#if WITH_EDITOR
		if(!CheckValidity(FocusList[i])){
			continue;
		}
		#endif

		const bool bIsFocused = FocusList[i]->HasKeyboardFocus();

		bIsFocused ? Cast<IUIFocus>(FocusList[i])->ReceiveFocus() : Cast<IUIFocus>(FocusList[i])->UnReceiveFocus();

		if(bIsFocused){
			LastFocusWidgetIndex = i;
		}
	}
}

#if WITH_EDITOR
bool UCustomUserWidget::CheckValidity(const UWidget* WidgetToCheck) const{
	const bool bIsValid = WidgetToCheck->GetClass()->ImplementsInterface(UUIFocus::StaticClass());

	if(!bIsValid){
		const auto ClassName = WidgetToCheck->GetClass()->GetName();

		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow,FString::Printf(TEXT("%s does not implement UIFocus interface"), *ClassName));
		UE_LOG(LogTemp, Warning, TEXT("%s does not implement UIFocus interface"), *ClassName);
	}

	return bIsValid;
}
#endif

void UCustomUserWidget::UnFocusAll(){
	bIsFocusNeeded = false;

	for(int i = 0; i < FocusList.Num(); i++){

		#if WITH_EDITOR
		if(!CheckValidity(FocusList[i])){
			continue;
		}
		#endif

		Cast<IUIFocus>(FocusList[i])->UnReceiveFocus();
	}
}

void UCustomUserWidget::Refocus(){
	bIsFocusNeeded = true;

	FocusList[LastFocusWidgetIndex]->SetKeyboardFocus();
	FocusWidgets();
}

void UCustomUserWidget::OnAnyKey(FKey KeyMap){
	// keyboard or gamepad key
	if((KeyMap.IsGamepadKey() && !LastPressedKey.IsGamepadKey()) || (UKismetInputLibrary::Key_IsKeyboardKey(KeyMap) && !UKismetInputLibrary::Key_IsKeyboardKey(LastPressedKey))){
		Refocus();
	}

	// mouse key
	else if(KeyMap.IsMouseButton() && !LastPressedKey.IsMouseButton()){
		UnFocusAll();
	}

	LastPressedKey = KeyMap;
}

void UCustomUserWidget::AddWidgetToFocusList(UWidget* WidgetToAdd){
	FocusList.Add(WidgetToAdd);
}

void UCustomUserWidget::AddWidgetsToFocusList(TArray<UWidget*> WidgetsToAdd){
	for(int i = 0; i < WidgetsToAdd.Num(); i++){
		FocusList.Add(WidgetsToAdd[i]);
	}
}

void UCustomUserWidget::RemoveWidgetToFocusList(UWidget* WidgetToRemove){
	FocusList.Remove(WidgetToRemove);
}

void UCustomUserWidget::RemoveWidgetsToFocusList(TArray<UWidget*> WidgetsToRemove){
	for(int i = 0; i < WidgetsToRemove.Num(); i++){
		FocusList.Remove(WidgetsToRemove[i]);
	}
}

TArray<UWidget*> UCustomUserWidget::GetFocusList() const{
	return FocusList;
}
