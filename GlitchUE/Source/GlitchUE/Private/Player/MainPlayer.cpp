// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/MainPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GlitchUEGameMode.h"
#include "PopcornFXAttributeFunctions.h"
#include "PopcornFXEmitterComponent.h"
#include "PopcornFXFunctions.h"
#include "AI/MainAICharacter.h"
#include "Player/MainPlayerController.h"
#include "Mark/Mark.h"
#include "Sound/SoundBase.h"

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

	GetMesh()->SetReceivesDecals(false);

	#pragma region Timelines

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_ZeroToOneCurve"));
	check(Curve.Succeeded());

	ZeroToOneCurve = Curve.Object;

	#pragma endregion

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> FX(TEXT("/Game/VFX/Particles/FX_Avatar/Pk_TPDash"));
	check(FX.Succeeded());

	GlichDashFXReference = FX.Object;
}


void AMainPlayer::BeginPlay(){
	Super::BeginPlay();

	MainPlayerController = Cast<AMainPlayerController>(GetController());

	FOnTimelineFloat UpdateEvent;
	FOnTimelineEvent FinishedEvent;

	UpdateEvent.BindDynamic(this, &AMainPlayer::LookAtMark);
	FinishedEvent.BindDynamic(this, &AMainPlayer::EndTL);

	CameraTransitionTL.AddInterpFloat(ZeroToOneCurve, UpdateEvent);
	CameraTransitionTL.SetTimelineFinishedFunc(FinishedEvent);

	UpdateEvent.Unbind();
	FinishedEvent.Unbind();
	UpdateEvent.BindDynamic(this, &AMainPlayer::CameraAimUpdate);
	FinishedEvent.BindDynamic(this, &AMainPlayer::CameraAimFinished);

	CameraAimTransition.AddInterpFloat(ZeroToOneCurve, UpdateEvent);
	CameraAimTransition.SetTimelineFinishedFunc(FinishedEvent);

	UpdateEvent.Unbind();
	UpdateEvent.BindDynamic(this, &AMainPlayer::CameraZoomUpdate);

	CameraZoomTransition.AddInterpFloat(ZeroToOneCurve, UpdateEvent);

	UpdateEvent.Unbind();
	UpdateEvent.BindDynamic(this, &AMainPlayer::CameraFOVUpdate);

	CameraFOVTransition.AddInterpFloat(ZeroToOneCurve, UpdateEvent);

	TArray<AActor*> NexusArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANexus::StaticClass(), NexusArray);
	Nexus = Cast<ANexus>(NexusArray[0]);

	TArray<AActor*> PreviewPlacableArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APreviewPlacableActor::StaticClass(), PreviewPlacableArray);
	PreviewPlacableActor = Cast<APreviewPlacableActor>(PreviewPlacableArray[0]);

	PreviewPlacableActor->GetPreviewMesh()->SetVectorParameterValueOnMaterials("Color", FVector(1, 0, 0));

	#pragma region FXCreation

	GlitchDashFX = UPopcornFXFunctions::SpawnEmitterAtLocation(GetWorld(), GlichDashFXReference, "PopcornFX_DefaultScene", FVector::ZeroVector, FRotator::ZeroRotator, false, false);
	GlitchDashFXBackup = UPopcornFXFunctions::SpawnEmitterAtLocation(GetWorld(), GlichDashFXReference, "PopcornFX_DefaultScene", FVector::ZeroVector, FRotator::ZeroRotator, false, false);

	const int TargetIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(GlitchDashFX, "TeleportLifetime");

	UPopcornFXAttributeFunctions::SetAttributeAsFloat(GlitchDashFX, TargetIndex, GlitchDashDuration + 1, true);
	UPopcornFXAttributeFunctions::SetAttributeAsFloat(GlitchDashFXBackup, TargetIndex, GlitchDashDuration + 1, true);

	#pragma endregion
}

void AMainPlayer::InitializePlayer(const FTransform StartTransform, const FRotator CameraRotation){
	SetActorTransform(StartTransform);
	Controller->SetControlRotation(CameraRotation);
}

#pragma region Camera

void AMainPlayer::CameraAim_Implementation(){
	CameraAimTransition.Play();
	CameraAimTimelineDirection = ETimelineDirection::Forward;
}

void AMainPlayer::CameraAimReverse_Implementation(){
	CameraAimTransition.Reverse();
	CameraAimTimelineDirection = ETimelineDirection::Backward;
}

void AMainPlayer::CameraStopAim(){
	CameraAimTransition.Stop();
}

void AMainPlayer::CameraAimUpdate_Implementation(float Alpha){
	CameraBoom->SocketOffset = UKismetMathLibrary::VLerp(FVector::ZeroVector, AimOffset, Alpha);
}

void AMainPlayer::CameraAimFinished_Implementation(){}

ETimelineDirection::Type AMainPlayer::GetCameraAimDirection() const{
	return CameraAimTimelineDirection;
}

void AMainPlayer::CameraZoom(float TargetZoom){
	TargetZoomValue = TargetZoom;
	CurrentZoomValue = CameraBoom->TargetArmLength;
	CameraZoomTransition.PlayFromStart();
}

void AMainPlayer::CameraZoomUpdate(float Alpha){
	CameraBoom->TargetArmLength = FMath::Lerp(CurrentZoomValue, TargetZoomValue, Alpha);
}

void AMainPlayer::CameraFOV(float TargetFOV){
	TargetFOVValue = TargetFOV;
	CurrentFOVValue = FollowCamera->FieldOfView;
	CameraZoomTransition.PlayFromStart();
}

void AMainPlayer::CameraFOVUpdate(float Alpha){
	FollowCamera->FieldOfView = FMath::Lerp(CurrentFOVValue, TargetFOVValue, Alpha);
}

void AMainPlayer::SetPlacableActorData(UPlacableActorData* Data){
	CurrentPlacableActorData = Data;
	PreviewPlacableActor->SetData(CurrentPlacableActorData);
}

UPlacableActorData* AMainPlayer::GetCurrentPlacableActorData() const{
	return CurrentPlacableActorData;
}

void AMainPlayer::GiveGolds_Implementation(int Amount){
	Golds = FMath::Clamp((Amount + Golds), 0, 999999);
}

#pragma endregion

#pragma region Interaction

bool AMainPlayer::InteractionLineTrace(FHitResult& OutHit) const{
	const FCollisionQueryParams QueryParams;
	const FCollisionResponseParams ResponseParam;
	return GetWorld()->LineTraceSingleByChannel(OutHit, FollowCamera->GetComponentLocation(), (FollowCamera->GetForwardVector() * InteractionLength) + FollowCamera->GetComponentLocation(), ECollisionChannel::ECC_Visibility, QueryParams, ResponseParam);
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

	UInteractableComponent* HittedInteractable = Cast<UInteractableComponent>(HitResult.Actor->GetComponentByClass(UInteractableComponent::StaticClass()));
	
	if (HittedInteractable == nullptr) {
		UnfeedbackCurrentCheckedObject();
		return;
	}

	if ((HittedInteractable != CurrentCheckedObject) && (HittedInteractable->CheckComponent(HitResult.GetComponent()))){
		CurrentCheckedObject = HittedInteractable;
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

void AMainPlayer::TurnAtRate(const float Rate){
	// calculate delta for this frame from the rate 
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainPlayer::LookUpAtRate(const float Rate){
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

EPlayerMovementMode AMainPlayer::GetMovementMode() const{
	return MovementMode;
}

void AMainPlayer::SetMovementMode(const EPlayerMovementMode NewMovementMode){
	MovementMode = NewMovementMode;
}

void AMainPlayer::MoveForward(const float Value){
	if ((Controller != nullptr) && (Value != 0.0f)){
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMainPlayer::MoveRight(const float Value){
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
	const FCollisionQueryParams QueryParams;
	const FCollisionResponseParams ResponseParam;
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

UHealthComponent* AMainPlayer::GetHealthComp(){
	return HealthComp;
}

void AMainPlayer::SetInGlitchZone(const bool bNewValue){
	bIsInGlitchZone = bNewValue;
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
	CameraAimReverse();

	HealthComp->SetCanTakeDamages(false);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	// Play sound start of the teleportation
	UGameplayStatics::SpawnSound2D(this, TPStart);

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

void AMainPlayer::UseGlitchPressed() {
	CameraAim();
}

void AMainPlayer::UseGlitchReleassed() {
	CameraStopAim();
	LaunchMark();

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		CameraAimReverse();
	}, 1.0f, false);
}

void AMainPlayer::Tick(float deltaTime){
	Super::Tick(deltaTime);
	
	CameraTransitionTL.TickTimeline(deltaTime);
	CameraAimTransition.TickTimeline(deltaTime);
	CameraZoomTransition.TickTimeline(deltaTime);
	CameraFOVTransition.TickTimeline(deltaTime);
}

void AMainPlayer::SetMark(AMark* NewMark) {
	Mark = NewMark;
}

void AMainPlayer::LookAtMark(float Value){	
	MainPlayerController->SetControlRotation(UKismetMathLibrary::RLerp(CurrentControlRotation, TargetControlRotation, Value, true));
}

void AMainPlayer::StartGlitchDashFX(){
	UPopcornFXEmitterComponent* FXToStart;
	// choisi le FX de backup si le principal est déjà utilisé
	GlitchDashFX->IsEmitterStarted() ? FXToStart = GlitchDashFXBackup : FXToStart = GlitchDashFX;

	const int PositionAIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(FXToStart, "PositionA");
	const int PositionBIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(FXToStart, "PositionB");

	UPopcornFXAttributeFunctions::SetAttributeAsVector(GlitchDashFX, PositionAIndex, GetActorLocation(), true);
	UPopcornFXAttributeFunctions::SetAttributeAsVector(GlitchDashFX, PositionBIndex, Mark->GetActorLocation(), true);

	FXToStart->StartEmitter();
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
	OverlappedAICharacters.Empty();

	TArray<AActor*> ActorToIgnore;
	ActorToIgnore.Add(Mark);
	
	TArray<FHitResult> HitResultList;

	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), GetActorLocation(), Mark->GetActorLocation(), FVector(25, 25, 25), FRotator::ZeroRotator, UEngineTypes::ConvertToTraceType(ECC_Camera), false, ActorToIgnore, EDrawDebugTrace::None, HitResultList, true, FLinearColor::Red, FLinearColor::Green, 1);
	
	for (int i = 0; i < HitResultList.Num(); i++) {
		if (HitResultList[i].GetActor()->IsA(AMainAICharacter::StaticClass())) {
			AMainAICharacter* CurrentAICharacter = Cast<AMainAICharacter>(HitResultList[i].GetActor());

			if (!OverlappedAICharacters.Contains(CurrentAICharacter)) {
				OverlappedAICharacters.Add(CurrentAICharacter);
				CurrentAICharacter->StunAI();
			}
			
		}
	}
}

void AMainPlayer::ResetOverlappedMeshes(){
	TArray<UStaticMeshComponent*> OverlappedMeshesArray = OverlappedMeshes.Array();

	for (int i = 0; i < OverlappedMeshesArray.Num(); i++) {
		OverlappedMeshesArray[i]->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, OverlappedMeshesCollisionResponse[i]);
	}

	OverlappedMeshes.Empty();
	OverlappedMeshesCollisionResponse.Empty();
}

void AMainPlayer::ReciveGlitchUpgrade(){
	IGlitchInterface::ReciveGlitchUpgrade();

	GetCharacterMovement()->MaxWalkSpeed = 800;
	
	FTimerHandle TimerHandle;

	GetWorldTimerManager().SetTimer(TimerHandle, [&]() {
		ResetGlitchUpgrade();
	}, GlitchUpgradeDuration, false);
}

void AMainPlayer::ResetGlitchUpgrade(){
	IGlitchInterface::ReciveGlitchUpgrade();
	
	GetCharacterMovement()->MaxWalkSpeed = 550;
}

void AMainPlayer::EndTL() {

	// Play sound final of the teleportation
	UGameplayStatics::SpawnSound2D(this, TPFinal);

	GlitchCameraTrace();

	GlitchTrace();

	Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->AddGlitch(GlitchDashValue + OverlappedMeshes.Num() + OverlappedAICharacters.Num());
	UE_LOG(LogTemp, Warning, TEXT("AddedGlitchDashValue : %f"), GlitchDashValue + OverlappedMeshes.Num() + OverlappedAICharacters.Num());

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