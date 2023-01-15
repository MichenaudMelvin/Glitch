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
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GlitchUEGameMode.h"

//////////////////////////////////////////////////////////////////////////
// AMainPlayer

AMainPlayer::AMainPlayer(){
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	PrimaryActorTick.bCanEverTick = true;

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

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	#pragma region Timelines

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/ZeroToOneCurve"));
	check(Curve.Succeeded());

	ZeroToOneCurve = Curve.Object;

	#pragma endregion
}


void AMainPlayer::BeginPlay(){
	Super::BeginPlay();

	MainPlayerController = Cast<AMainPlayerController>(GetController());

	FOnTimelineFloat UpdateEvent = FOnTimelineFloat();
	FOnTimelineEventStatic FinishedEvent = FOnTimelineEventStatic();

	UpdateEvent.BindUFunction(this, FName{ TEXT("LookAtMark") });
	FinishedEvent.BindUFunction(this, FName{ TEXT("EndTL") });

	CameraTransitionTL.AddInterpFloat(ZeroToOneCurve, UpdateEvent);
	CameraTransitionTL.SetTimelineFinishedFunc(FinishedEvent);

	//UpdateEvent.Clear();
	//FinishedEvent.Unbind();
	//UpdateEvent.BindUFunction(this, FName{ TEXT("LookAtMark") });
	//FinishedEvent.BindUFunction(this, FName{ TEXT("EndTL") });

	/*
	CameraTransitionTL->AddInterpFloat(ZeroToOneCurve, UpdateEvent);
	CameraTransitionTL->SetTimelineFinishedFunc(FinishedEvent);
	CameraTransitionTL->RegisterComponentWithWorld(GetWorld());
	*/
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

void AMainPlayer::InteractionTick(){
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

void AMainPlayer::SneakPressed_Implementation(){}

void AMainPlayer::SneakReleased_Implementation(){}

void AMainPlayer::SprintToSneak_Implementation(){}

void AMainPlayer::ResetMovement_Implementation(){}

EPlayerMovementMode AMainPlayer::GetMovementMode(){
	return MovementMode;
}

void AMainPlayer::SetMovementMode(EPlayerMovementMode NewMovementMode){
	MovementMode = NewMovementMode;
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
	FCollisionResponseParams ResponseParam;
	if (GetWorld()->LineTraceSingleByChannel(Hit, FollowCamera->GetComponentLocation(), (FollowCamera->GetForwardVector() * InteractionLength) + FollowCamera->GetComponentLocation(), ECollisionChannel::ECC_Visibility, QueryParams, ResponseParam) && PreviewPlacableActor->CanBePlaced()){
		PlacableActorLocation = Hit.Location;
		PreviewPlacableActor->SetActorLocation(PlacableActorLocation.GridSnap(100));
	} else {
		PreviewPlacableActor->ResetActor();
	}
}


void AMainPlayer::PlaceObject(){
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(PlacableActorLocation);
	FActorSpawnParameters ActorsSpawnParameters;
	GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), SpawnTransform, ActorsSpawnParameters);
}

AMainPlayerController* AMainPlayer::GetMainPlayerController() {
	return MainPlayerController;
}

#pragma region Mark

void AMainPlayer::LaunchMark(){
	FTransform MarkTransform;
	MarkTransform.SetLocation(GetActorLocation());
	MarkTransform.SetRotation(FindMarkLaunchRotation());
	MarkTransform.SetScale3D(FVector::OneVector * 0.1f);
	Mark->Launch(MarkTransform);

	MainPlayerController->UnbindGlitch();
	MainPlayerController->OnUseGlitchPressed.AddDynamic(this, &AMainPlayer::TPToMark);
}

FQuat AMainPlayer::FindMarkLaunchRotation(){
	FHitResult HitResult;
	FVector StartLocation = FollowCamera->GetComponentLocation();
	FVector EndLocation = (FollowCamera->GetForwardVector() * Mark->GetMaxDistance()) + StartLocation;

	FCollisionQueryParams QueryParams;
	FCollisionResponseParams ResponseParam;

	FVector TargetRotation = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, QueryParams, ResponseParam) ? HitResult.ImpactPoint : EndLocation;
	return UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetRotation).Quaternion();
}

void AMainPlayer::TPToMark() {

	MainPlayerController->UnbindMovement();
	MainPlayerController->UnbindCamera();
	MainPlayerController->UnbindGlitch();

	StopJumping();
	Mark->PlaceMark();
	// camera aim reverse
	HealthComp->SetCanTakeDamages(false);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	FRotator CapsuleRotation = FRotator::ZeroRotator;
	CapsuleRotation.Yaw = Mark->GetActorRotation().Yaw;
	GetCapsuleComponent()->SetRelativeRotation(CapsuleRotation);
	GetMesh()->SetVisibility(false, true);
	
	CurrentControlRotation = MainPlayerController->GetControlRotation();
	CurrentCameraPosition = FollowCamera->GetComponentLocation();
	TargetControlRotation = UKismetMathLibrary::FindLookAtRotation(CurrentCameraPosition, Mark->GetActorLocation());

	StartGlitchDashFX();

	CameraTransitionTL.PlayFromStart();
}

void AMainPlayer::UseGlitchPressed_Implementation() {}

void AMainPlayer::UseGlitchReleassed_Implementation() {}

void AMainPlayer::Tick(float deltaTime){
	Super::Tick(deltaTime);
	
	CameraTransitionTL.TickTimeline(deltaTime);
}

void AMainPlayer::SetMark(AMark* NewMark) {
	Mark = NewMark;
}

void AMainPlayer::LookAtMark(float Value){	
	MainPlayerController->SetControlRotation(UKismetMathLibrary::RLerp(CurrentControlRotation, TargetControlRotation, Value, true));
}

void AMainPlayer::StartGlitchDashFX_Implementation(){
	//class UPopcornFXAttributeFunctions::SetAttributeAsFloat(GlitchDashFX, );
}

void AMainPlayer::GlitchCameraTrace(){
	FVector BoxHalfSize = FVector::OneVector;
	BoxHalfSize *= 50;

	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> HitResultList;

	UKismetSystemLibrary::BoxTraceMulti(this, FollowCamera->GetComponentLocation(), Mark->GetActorLocation(), BoxHalfSize, FRotator::ZeroRotator, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), false, ActorsToIgnore, EDrawDebugTrace::None, HitResultList, true, FColor::Red, FColor::Green, 0.5f);

	for (int i = 0; i < HitResultList.Num(); i++) {
		if (HitResultList[i].GetComponent()->IsA(UStaticMeshComponent::StaticClass())) {
			UStaticMeshComponent* CurrentStaticMeshComp = Cast<UStaticMeshComponent>(HitResultList[i].GetComponent());
			
			if (!OverlappedMeshes.Contains(CurrentStaticMeshComp)){
				OverlappedMeshes.Add(CurrentStaticMeshComp);
				OverlappedMeshesCollisionResponse.Add(CurrentStaticMeshComp->GetCollisionResponseToChannel(ECollisionChannel::ECC_Camera));
				CurrentStaticMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
			}
		}
	}
}

void AMainPlayer::GlitchTrace(){

}

void AMainPlayer::ResetOverlappedMeshes(){
	TArray<UStaticMeshComponent*> OverlappedMeshesArray = OverlappedMeshes.Array();

	for (int i = 0; i < OverlappedMeshesArray.Num(); i++) {
		OverlappedMeshesArray[i]->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, OverlappedMeshesCollisionResponse[i]);
	}

	OverlappedMeshes.Empty();
	OverlappedMeshesCollisionResponse.Empty();
}

void AMainPlayer::EndTL() {
	GlitchCameraTrace();

	GlitchTrace();

	Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->AddGlitch(GlitchDashValue + OverlappedMeshes.Num());
	UE_LOG(LogTemp, Warning, TEXT("AddedGlitchDashValue : %f"), GlitchDashValue + OverlappedMeshes.Num());

	ResetMovement();

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;

	UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), Mark->GetTPLocation(), GetCapsuleComponent()->GetRelativeRotation(), false, false, 0.2f, false, EMoveComponentAction::Type::Move, LatentInfo);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){

		MainPlayerController->BindMovement();
		MainPlayerController->BindCamera();

		ResetOverlappedMeshes();

		Mark->ResetMark();
		GetMesh()->SetVisibility(true, true);
		GetCharacterMovement()->GravityScale = 1;

		HealthComp->SetCanTakeDamages(true);
	}, 0.2f, false);
}

#pragma endregion

void AMainPlayer::TestFunction(){
	UE_LOG(LogTemp, Warning, TEXT("Test function called"));
}