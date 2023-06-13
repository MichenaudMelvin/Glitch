// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Waypoint.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h"
#include "Player/MainPlayerController.h"
#include "Kismet/GameplayStatics.h"

UWaypoint::UWaypoint(){
	PrimaryComponentTick.bCanEverTick = true;
}

void UWaypoint::BeginPlay(){
	Super::BeginPlay();

	Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	DrawWaypoint(bDrawWayPoint);
}

void UWaypoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckComponentRender();
}

bool UWaypoint::IsVisibleOnScreen() const{
	FVector2D ScreenPosition;
	UGameplayStatics::ProjectWorldToScreen(MainPlayerController, GetComponentLocation(), ScreenPosition);

	const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetOwner());

	const bool bOnScreen = ScreenPosition > FVector2D::ZeroVector && ScreenPosition < ViewportSize;

	return bOnScreen;
}

void UWaypoint::CheckComponentRender(){
	#if WITH_EDITOR
		// for simulation mode
		if(!IsValid(MainPlayerController)){
			return;
		}
	#endif

	if(!bDrawWayPoint || !IsVisible()){
		MainPlayerController->GetWaypointIndicationWidget()->RemoveIndication(this);
		return;
	}

	if(IsVisibleOnScreen()){
		MainPlayerController->GetWaypointIndicationWidget()->RemoveIndication(this);
	}

	else if(!IsVisibleOnScreen()){
		MainPlayerController->GetWaypointIndicationWidget()->AddIndication(this);
	}
}

bool UWaypoint::IsAtLeft() const{
	FVector2D ScreenPosition;
	UGameplayStatics::ProjectWorldToScreen(MainPlayerController, GetComponentLocation(), ScreenPosition);

	const float HorizontalViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetOwner()).X/2;

	return ScreenPosition.X < HorizontalViewportSize;
}

void UWaypoint::SetImage(UImage* NewImage){
	ImageRef = NewImage;
}

UImage* UWaypoint::GetImage() const{
	return ImageRef;
}

void UWaypoint::DrawWaypoint(const bool bShouldDrawWaypoint){
	bDrawWayPoint = bShouldDrawWaypoint;
	SetVisibility(bDrawWayPoint);
}
