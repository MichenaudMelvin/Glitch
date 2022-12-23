// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/MainPlayer.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

//////////////////////////////////////////////////////////////////////////
// AMainPlayer

AMainPlayer::AMainPlayer(){
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AMainPlayer::GiveGolds(int Amount){
	Golds = FMath::Clamp((Amount + Golds), 0, 999999);
}

#pragma region Interaction




bool AMainPlayer::InteractionLineTrace(FHitResult& outHitResult){
	FCollisionQueryParams QueryParams;
	FCollisionResponseParams Responseparam;
	return GetWorld()->LineTraceSingleByChannel(outHitResult, FollowCamera->GetComponentLocation(), (FollowCamera->GetForwardVector() * InteractionLength) + FollowCamera->GetComponentLocation(), ECollisionChannel::ECC_Visibility, QueryParams, Responseparam);
}

void AMainPlayer::InteractionTick_Implementation(){
	FHitResult HitResult;

	if (!InteractionLineTrace(HitResult)) {
		UnfeedbackCurrentCheckedObject();
		return;
	}

	if (HitResult.Actor == nullptr) {
		UnfeedbackCurrentCheckedObject();
		return;
	}

	UInteractableComponent* hittedInteractable = Cast<UInteractableComponent>(HitResult.Actor->GetComponentByClass(UInteractableComponent::StaticClass()));
	
	if (hittedInteractable == nullptr) {
		UnfeedbackCurrentCheckedObject();
		return;
	}

	if ((hittedInteractable != CurrentCheckedObject) && (hittedInteractable->CheckComponent(HitResult.GetComponent()))){
		CurrentCheckedObject = hittedInteractable;
		CurrentCheckedObject->Feedback();
	}
}

void AMainPlayer::Interact() {
	if (IsValid(CurrentCheckedObject)) {
		CurrentCheckedObject->Interact(MainPlayerController, this);
	}
}

void AMainPlayer::UnfeedbackCurrentCheckedObject() {
	if (IsValid(CurrentCheckedObject)) {
		CurrentCheckedObject->Unfeedback();
	}

	CurrentCheckedObject = nullptr;
}

#pragma endregion

void AMainPlayer::TurnAtRate(float Rate){
	// calculate delta for this frame from the rate 
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainPlayer::LookUpAtRate(float Rate){
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMainPlayer::AddControllerPitchInput(float Rate){
	if (bInvertYAxis) {
		Rate = Rate * -1;
	}
	Super::AddControllerPitchInput(Rate);
}

void AMainPlayer::MoveForward(float Value){
	if ((Controller != nullptr) && (Value != 0.0f)){
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMainPlayer::MoveRight(float Value){
	if ( (Controller != nullptr) && (Value != 0.0f) ){
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AMainPlayer::PreviewObject(){
	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	FCollisionResponseParams Responseparam;
	if (GetWorld()->LineTraceSingleByChannel(Hit, FollowCamera->GetComponentLocation(), (FollowCamera->GetForwardVector() * InteractionLength) + FollowCamera->GetComponentLocation(), ECollisionChannel::ECC_Visibility, QueryParams, Responseparam) && PlacableActor->PreviewObject()){
		PlacableActorLocation = Hit.Location;
		PlacableActor->PlaceObject(PlacableActorLocation.GridSnap(100));
	} else {
		PlacableActor->PlaceObject(PlacableActor->GetOriginalLocation());
	}
}


void AMainPlayer::PlaceObject(){
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(PlacableActorLocation);
	FActorSpawnParameters ActorsSpawnParameters;
	GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), SpawnTransform, ActorsSpawnParameters);
}