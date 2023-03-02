// Fill out your copyright notice in the Description page of Project Settings.


#include "Helpers/Debug/DebugPawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Player/MainPlayerController.h"

ADebugPawn::ADebugPawn(){
 	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComponent);
	
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComp"));
	MovementComponent->UpdatedComponent = RootComponent;

	CustomTimeDilation = 1;
}

void ADebugPawn::BeginPlay(){
	Super::BeginPlay();

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		MainPlayerController = Cast<AMainPlayerController>(GetController());
		MainPlayerController->UnbindAll();

		MainPlayerController->OnMoveForward.AddDynamic(this, &ADebugPawn::MoveForward);
		MainPlayerController->OnMoveRight.AddDynamic(this, &ADebugPawn::MoveRight);
		MainPlayerController->OnMouseScroll.AddDynamic(this, &ADebugPawn::ManageSpeed);
		MainPlayerController->OnUseGlitchPressed.AddDynamic(this, &ADebugPawn::PanPressed);
		MainPlayerController->OnUseGlitchReleased.AddDynamic(this, &ADebugPawn::PanReleassed);
	}, 0.1f, false);
}

void ADebugPawn::MoveForward(float AxisValue){
	AddMovementInput(GetControlRotation().Vector(), AxisValue);
}

void ADebugPawn::MoveRight(float AxisValue){
	AddMovementInput(FRotationMatrix(FRotator(GetControlRotation().Pitch, GetControlRotation().Yaw, GetControlRotation().Roll)).GetScaledAxis(EAxis::Y), AxisValue);
}

void ADebugPawn::PanPressed(){
	MainPlayerController->OnLookUp.AddDynamic(this, &ADebugPawn::LookUp);
	MainPlayerController->OnTurn.AddDynamic(this, &ADebugPawn::Turn);
}

void ADebugPawn::PanReleassed(){
	MainPlayerController->OnLookUp.Clear();
	MainPlayerController->OnTurn.Clear();
}

void ADebugPawn::LookUp(float AxisValue){
	AddControllerPitchInput(AxisValue);
}

void ADebugPawn::Turn(float AxisValue){
	AddControllerYawInput(AxisValue);
}

void ADebugPawn::ManageSpeed(float AxisValue){
	MovementComponent->MaxSpeed = FMath::Clamp(MovementComponent->MaxSpeed + (AxisValue * 500), 0.0f, 999999999999.0f);
}