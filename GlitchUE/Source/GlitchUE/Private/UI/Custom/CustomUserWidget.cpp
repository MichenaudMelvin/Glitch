// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Custom/CustomUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UIFocus.h"

void UCustomUserWidget::NativeOnInitialized(){
	Super::NativeOnInitialized();

	CurrentController = Cast<AAbstractPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void UCustomUserWidget::NativeConstruct(){
	Super::NativeConstruct();

	CurrentController->OnAnyKey.AddDynamic(this, &UCustomUserWidget::OnAnyKey);

	if(FocusList.Num() > 0 && IsValid(FocusList[LastFocusWidgetIndex])){
		// for some reasons a timer is needed
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){
			Refocus();
		}, 0.1f, false);
	}
}

void UCustomUserWidget::NativeDestruct(){
	CurrentController->OnAnyKey.RemoveDynamic(this, &UCustomUserWidget::OnAnyKey);

	// if the last focused widget was remove from the focus list
	if(!FocusList.IsValidIndex(LastFocusWidgetIndex)){
		Super::NativeDestruct();
		return;
	}

	#if WITH_EDITOR
	if(!FocusList[LastFocusWidgetIndex]->GetClass()->ImplementsInterface(UUIFocus::StaticClass())){
		const auto ClassName = FocusList[LastFocusWidgetIndex]->GetClass()->GetName();

		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow,FString::Printf(TEXT("%s does not implement UIFocus interface"), *ClassName));
		UE_LOG(LogTemp, Warning, TEXT("%s does not implement UIFocus interface"), *ClassName);
	}
	#endif

	//Cast<IUIFocus>(FocusList[LastFocusWidgetIndex])->UnReceiveFocus();

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
		if(!FocusList[i]->GetClass()->ImplementsInterface(UUIFocus::StaticClass())){
			const auto ClassName = FocusList[i]->GetClass()->GetName();

			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow,FString::Printf(TEXT("%s does not implement UIFocus interface"), *ClassName));
			UE_LOG(LogTemp, Warning, TEXT("%s does not implement UIFocus interface"), *ClassName);
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

void UCustomUserWidget::UnFocusAll(){
	bIsFocusNeeded = false;

	for(int i = 0; i < FocusList.Num(); i++){
		#if WITH_EDITOR
		if(!FocusList[i]->GetClass()->ImplementsInterface(UUIFocus::StaticClass())){
			const auto ClassName = FocusList[i]->GetClass()->GetName();

			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow,FString::Printf(TEXT("%s does not implement UIFocus interface"), *ClassName));
			UE_LOG(LogTemp, Warning, TEXT("%s does not implement UIFocus interface"), *ClassName);
			continue;
		}
		#endif

		Cast<IUIFocus>(FocusList[i])->UnReceiveFocus();
	}
}

void UCustomUserWidget::Refocus(){
	if(!FocusList.IsValidIndex(LastFocusWidgetIndex) || !IsValid(FocusList[LastFocusWidgetIndex])){
		return;
	}

	bIsFocusNeeded = true;

	FocusList[LastFocusWidgetIndex]->SetKeyboardFocus();
	FocusWidgets();
}

void UCustomUserWidget::OnAnyKey(FKey KeyMap){
	if(KeyMap.IsGamepadKey()){
		Refocus();
	}

	else if(KeyMap.IsMouseButton()){
		UnFocusAll();
	}

	else{
		Refocus();
	}
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
