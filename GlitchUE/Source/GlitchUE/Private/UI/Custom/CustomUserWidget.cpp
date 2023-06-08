// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Custom/CustomUserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Helpers/FunctionsLibrary/UsefulFunctions.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UIFocus.h"

void UCustomUserWidget::NativeOnInitialized(){
	Super::NativeOnInitialized();

	CurrentController = Cast<AAbstractPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void UCustomUserWidget::NativeConstruct(){
	Super::NativeConstruct();

	CurrentController->OnSwitchToKeyboard.AddDynamic(this, &UCustomUserWidget::UnFocusAll);
	CurrentController->OnSwitchToGamepad.AddDynamic(this, &UCustomUserWidget::Refocus);

	//CheckFocusAtStart();
	if(CurrentController->IsGameplaySaveValid()){
		bIsFocusNeeded = CurrentController->IsUsingGamepad();
	} else {
		bIsFocusNeeded = Cast<UGameplaySave>(UUsefulFunctions::LoadSave(UGameplaySave::StaticClass(), 0, true))->IsUsingGamepad();
	}

	if(!bIsFocusNeeded){
		return;
	}

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){
		Refocus();
	}, 0.1f, false);
}

void UCustomUserWidget::NativeDestruct(){
	CurrentController->OnSwitchToKeyboard.RemoveDynamic(this, &UCustomUserWidget::UnFocusAll);
	CurrentController->OnSwitchToGamepad.RemoveDynamic(this, &UCustomUserWidget::Refocus);

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

	if(bIsDynamicFocusList){
		FocusList.Empty();
	}

	Super::NativeDestruct();
}

void UCustomUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime){
	Super::NativeTick(MyGeometry, InDeltaTime);

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

void UCustomUserWidget::ResetLastFocusWidgetIndex(){
	LastFocusWidgetIndex = 0;
}
