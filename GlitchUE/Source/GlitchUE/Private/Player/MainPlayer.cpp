// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/MainPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GlitchUEGameMode.h"
#include "NavigationSystem.h"
#include "PopcornFXAttributeFunctions.h"
#include "PopcornFXEmitterComponent.h"
#include "PopcornFXFunctions.h"
#include "AI/MainAICharacter.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Player/MainPlayerController.h"
#include "Mark/Mark.h"
#include "Components/CompassComponent.h"
#include "AI/AIPursuitDrone/PursuitDrone.h"

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

	GetMesh()->SetReceivesDecals(false);
	GetMesh()->SetEnableGravity(false);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);

	Compass = CreateDefaultSubobject<UCompassComponent>(TEXT("Compass"));

	#pragma region Timelines

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_ZeroToOneCurve"));
	check(Curve.Succeeded());

	ZeroToOneCurve = Curve.Object;

	#pragma endregion

	RunFX = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("Run FX"));

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> RunEffect(TEXT("/Game/VFX/Particles/FX_Avatar/Pk_RunFX"));
	check(RunEffect.Succeeded());

	RunFX->SetEffect(RunEffect.Object);
	RunFX->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> FX(TEXT("/Game/VFX/Particles/FX_Avatar/Pk_TPDash"));
	check(FX.Succeeded());

	GlichDashFXReference = FX.Object;
}

void AMainPlayer::BeginPlay(){
	Super::BeginPlay();

	GameplaySettingsSaveSave = Cast<UGameplaySettingsSave>(UUsefullFunctions::LoadSave(UGameplaySettingsSave::StaticClass(), 0));
	FollowCamera->FieldOfView = GameplaySettingsSaveSave->CameraFOV;
	Sensibility = GameplaySettingsSaveSave->CameraSensibility;
	bInvertYAxis = GameplaySettingsSaveSave->bInvertCamYAxis;

	MainPlayerController = Cast<AMainPlayerController>(GetController());

	FOnTimelineFloat UpdateEvent;
	FOnTimelineEvent FinishedEvent;

	UpdateEvent.BindDynamic(this, &AMainPlayer::LookAtTargetUpdate);

	CameraTransitionTL.AddInterpFloat(ZeroToOneCurve, UpdateEvent);

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

	UpdateEvent.Unbind();
	FinishedEvent.Unbind();

	UpdateEvent.BindDynamic(this, &AMainPlayer::FadeInGlitchEffect);
	FinishedEvent.BindDynamic(this, &AMainPlayer::EndFadeIn);

	FadeInGlitchEffectTimeline.AddInterpFloat(ZeroToOneCurve, UpdateEvent);
	FadeInGlitchEffectTimeline.SetTimelineFinishedFunc(FinishedEvent);

	TArray<AActor*> NexusArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANexus::StaticClass(), NexusArray);
	Nexus = Cast<ANexus>(NexusArray[0]);

	TArray<AActor*> PreviewPlacableArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APreviewPlacableActor::StaticClass(), PreviewPlacableArray);
	PreviewPlacableActor = Cast<APreviewPlacableActor>(PreviewPlacableArray[0]);

	PreviewPlacableActor->GetPreviewMesh()->SetVectorParameterValueOnMaterials("Color", FVector(1, 0, 0));
	PreviewPlacableActor->SetPlayer(this);

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	GetCharacterMovement()->GravityScale = OriginalGravityScale;
	GetCharacterMovement()->GroundFriction = OriginalGroundFriction;
	GetCharacterMovement()->BrakingDecelerationWalking = OriginalBrakingDecelerationWalking;

	StartRecord();

	#pragma region FXCreation

	const int LifeTimeIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(RunFX, "Lifetime");
	const int LifeTimeDeviationIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(RunFX, "LifeTimeDeviation");

	UPopcornFXAttributeFunctions::GetAttributeAsFloat(RunFX, LifeTimeIndex, RunFXLifeTime, true);
	UPopcornFXAttributeFunctions::GetAttributeAsFloat(RunFX, LifeTimeDeviationIndex, RunFXLifeTimeDeviation, true);

	GlitchDashFX = UPopcornFXFunctions::SpawnEmitterAtLocation(GetWorld(), GlichDashFXReference, "PopcornFX_DefaultScene", FVector::ZeroVector, FRotator::ZeroRotator, false, false);
	GlitchDashFXBackup = UPopcornFXFunctions::SpawnEmitterAtLocation(GetWorld(), GlichDashFXReference, "PopcornFX_DefaultScene", FVector::ZeroVector, FRotator::ZeroRotator, false, false);

	const int TargetIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(GlitchDashFX, "TeleportLifetime");

	UPopcornFXAttributeFunctions::SetAttributeAsFloat(GlitchDashFX, TargetIndex, GlitchDashDuration + 1, true);
	UPopcornFXAttributeFunctions::SetAttributeAsFloat(GlitchDashFXBackup, TargetIndex, GlitchDashDuration + 1, true);

	#pragma endregion
}

void AMainPlayer::Destroyed(){
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	Super::Destroyed();
}

void AMainPlayer::InitializePlayer(const FTransform StartTransform, const FRotator CameraRotation, const FTransform MarkTransform, const bool bIsMarkPlaced){
	SetActorTransform(StartTransform);
	Controller->SetControlRotation(CameraRotation);

	if(bIsMarkPlaced){
		Mark->SetActorTransform(MarkTransform);
		Mark->PlaceMark();
	}
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

int AMainPlayer::GetGolds() const{
	return Golds;
}

void AMainPlayer::UpdateGolds(int Amount, const EGoldsUpdateMethod GoldsUpdateMethod){
	if(!bGoldsCanBeUpdated){
		return;
	}

	switch (GoldsUpdateMethod){
		case EGoldsUpdateMethod::BuyPlacable:
			// code here
			break;
		case EGoldsUpdateMethod::TakeDamages:
			Amount = - Amount;
			break;
		case EGoldsUpdateMethod::ReceiveGolds:
			// code here
			break;
	}

	Golds += Amount;
	MainPlayerController->GetPlayerStatsWidget()->UpdateDisplayGolds(Golds);

	if(Golds < 0){
		Loose();
	}
}

void AMainPlayer::SetGolds(const int Amount){
	Golds = Amount;
	MainPlayerController->GetPlayerStatsWidget()->UpdateDisplayGolds(Golds);
}

void AMainPlayer::KillPlayer(){
	UpdateGolds(Golds + 1, EGoldsUpdateMethod::TakeDamages);
}

bool AMainPlayer::CanUpdateGolds() const{
	return bGoldsCanBeUpdated;
}

void AMainPlayer::Loose_Implementation(){
	StopRecord();
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

void AMainPlayer::SetCurrentDrone(APursuitDrone* NewDrone){
	if(!IsValid(NewDrone)){
		CurrentDrone = nullptr;
		return;
	}

	if(IsValid(CurrentDrone)){
		DropDrone(NewDrone);
	}

	CurrentDrone = NewDrone;

	CurrentDrone->AttachDrone(this, "Bone012");

	
}

APursuitDrone* AMainPlayer::GetCurrentDrone() const{
	return CurrentDrone;
}

void AMainPlayer::DropDrone(APursuitDrone* NewDrone){
	const FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
	CurrentDrone->DetachFromActor(DetachmentRules);

	CurrentDrone->DisableSpinBehavior();

	CurrentDrone->SetActorLocation(NewDrone->GetActorLocation());

	CurrentDrone = nullptr;
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
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() * Sensibility);
}

void AMainPlayer::LookUpAtRate(const float Rate){
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() * Sensibility);
}

void AMainPlayer::Jump(){
	Super::Jump();

	MainPlayerController->UnbindSneak();
	UUsefullFunctions::MakeNoise(this, GetActorLocation(), JumpNoiseRange);

	if(bUseCoyoteTime){
		bUseCoyoteTime = false;
		GetCharacterMovement()->Velocity.Z = FMath::Max(GetCharacterMovement()->Velocity.Z, GetCharacterMovement()->JumpZVelocity);
	}
}

void AMainPlayer::OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal, const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta){
	Super::OnWalkingOffLedge_Implementation(PreviousFloorImpactNormal, PreviousFloorContactNormal, PreviousLocation,TimeDelta);

	bUseCoyoteTime = true;
	MainPlayerController->UnbindSneak();

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){
		bUseCoyoteTime = false;
	}, CoyoteTime, false);
}

void AMainPlayer::AddControllerYawInput(float Val){
	Super::AddControllerYawInput(Val * Sensibility);

	MakeMovementNoise();
}

void AMainPlayer::AddControllerPitchInput(float Rate){
	if (bInvertYAxis) {
		Rate = Rate * -1;
	}

	Super::AddControllerPitchInput(Rate * Sensibility);

	MakeMovementNoise();
}

void AMainPlayer::SneakPressed_Implementation(){}

void AMainPlayer::SneakReleased_Implementation(){}

void AMainPlayer::SprintToSneak_Implementation(){}

void AMainPlayer::ResetMovement_Implementation(){}

void AMainPlayer::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode){
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if(PrevMovementMode == MOVE_Falling){
		MainPlayerController->BindSneak();
	}
}

void AMainPlayer::MakeMovementNoise(){
	if(!UUsefullFunctions::IsCharacterMovingOnGround(this)){
		return;
	}

	float NoiseRadius = 0;

	switch (MovementMode) {
	case EPlayerMovementMode::Normal:
		NoiseRadius = NormalSpeedNoiseRange;
		break;
	case EPlayerMovementMode::Sneaking:
		NoiseRadius = CrouchSpeedNoiseRange;
		break;
	case EPlayerMovementMode::Sprinting:
		NoiseRadius = SprintSpeedNoiseRange;
		break;
	}

	UUsefullFunctions::MakeNoise(this, GetActorLocation(), NoiseRadius);
}

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

void AMainPlayer::SetInvertAxis(const bool bNewValue){
	bInvertYAxis = bNewValue;
}

void AMainPlayer::SetSensibility(const float NewSensibility){
	Sensibility = NewSensibility;
}


void AMainPlayer::PlaceObject(){
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(PlacableActorLocation);
	const FActorSpawnParameters ActorsSpawnParameters;
	GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), SpawnTransform, ActorsSpawnParameters);
}

AMainPlayerController* AMainPlayer::GetMainPlayerController() const{
	return MainPlayerController;
}

bool AMainPlayer::IsInGlitchZone() const{
	return bIsInGlitchZone;
}

void AMainPlayer::SetInGlitchZone(const bool bNewValue){
	bIsInGlitchZone = bNewValue;
}

#pragma region Mark

void AMainPlayer::LaunchMark(){
	FTransform MarkTransform;
	MarkTransform.SetLocation(GetMesh()->GetSocketLocation("Bone012"));

	MarkTransform.SetRotation(FindMarkLaunchRotation());
	MarkTransform.SetScale3D(FVector::OneVector * 0.1f);
	Mark->Launch(MarkTransform);

	MainPlayerController->UnbindGlitch();
	MainPlayerController->OnUseGlitchPressed.AddDynamic(this, &AMainPlayer::TPToMark);
}

FQuat AMainPlayer::FindMarkLaunchRotation() const{
	FHitResult HitResult;
	const FVector StartLocation = FollowCamera->GetComponentLocation();
	const FVector EndLocation = (FollowCamera->GetForwardVector() * Mark->GetMaxDistance()) + StartLocation;

	FCollisionQueryParams QueryParams;
	FCollisionResponseParams ResponseParam;

	FVector TargetLocation = EndLocation;

	if(GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams, ResponseParam)){
		TargetLocation = HitResult.ImpactPoint;
		//Mark->SetHitSomething(true);
	}

	Mark->SetTargetLocation(TargetLocation);

	return UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetSocketLocation("Bone012"), TargetLocation).Quaternion();
}

void AMainPlayer::TPToMark() {

	MainPlayerController->UnbindMovement();
	MainPlayerController->UnbindCamera();
	MainPlayerController->UnbindGlitch();
	MainPlayerController->SetCanSave(false);

	StopJumping();
	Mark->PlaceMark();
	CameraAimReverse();

	bGoldsCanBeUpdated = false;
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	// Play sound start of the teleportation
	UGameplayStatics::SpawnSound2D(this, TPStart);

	FRotator CapsuleRotation = FRotator::ZeroRotator;
	CapsuleRotation.Yaw = Mark->GetActorRotation().Yaw;
	GetCapsuleComponent()->SetRelativeRotation(CapsuleRotation);
	GetMesh()->SetVisibility(false, true);
	if(IsValid(CurrentDrone)){
		CurrentDrone->GetMesh()->SetVisibility(false, true);
	}

	CurrentCameraPosition = FollowCamera->GetComponentLocation();

	StartGlitchDashFX();

	FOnTimelineEvent FinishedEvent;

	FinishedEvent.BindDynamic(this, &AMainPlayer::EndTL);

	LookAtTarget(UKismetMathLibrary::FindLookAtRotation(CurrentCameraPosition, Mark->GetActorLocation()), FinishedEvent, GlitchDashCameraTransition);
}

void AMainPlayer::UseGlitchPressed(){
	CameraAim();
}

void AMainPlayer::UseGlitchReleassed(){
	CameraStopAim();
	LaunchMark();

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMainPlayer::CameraAimReverse, 1.0f, false);
}

void AMainPlayer::Tick(float deltaTime){
	Super::Tick(deltaTime);

	CameraTransitionTL.TickTimeline(deltaTime);
	CameraAimTransition.TickTimeline(deltaTime);
	CameraZoomTransition.TickTimeline(deltaTime);
	CameraFOVTransition.TickTimeline(deltaTime);
	FadeInGlitchEffectTimeline.TickTimeline(deltaTime);

	// Manage the run FX, disable it if the character is not on ground or not moving
	const int LifeTimeIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(RunFX, "Lifetime");
	const int LifeTimeDeviationIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(RunFX, "LifeTimeDeviation");

	if(GetVelocity().Size() > 0 && GetCharacterMovement()->IsMovingOnGround()){
		UPopcornFXAttributeFunctions::SetAttributeAsFloat(RunFX, LifeTimeIndex, RunFXLifeTime, true);
		UPopcornFXAttributeFunctions::SetAttributeAsFloat(RunFX, LifeTimeDeviationIndex, RunFXLifeTimeDeviation, true);

	} else if((GetVelocity().Size() <= 0 || !GetCharacterMovement()->IsMovingOnGround())){
		UPopcornFXAttributeFunctions::SetAttributeAsFloat(RunFX, LifeTimeIndex, 0, true);
		UPopcornFXAttributeFunctions::SetAttributeAsFloat(RunFX, LifeTimeDeviationIndex, 0, true);
	}
}

void AMainPlayer::SetMark(AMark* NewMark){
	Mark = NewMark;
}

void AMainPlayer::LookAtTarget(const FRotator TargetRotation, const FOnTimelineEvent FinishedEvent, const float Duration){
	TargetControlRotation = TargetRotation;
	CurrentControlRotation = MainPlayerController->GetControlRotation();

	CameraTransitionTL.SetTimelineFinishedFunc(FinishedEvent);
	CameraTransitionTL.SetPlayRate(1/Duration);

	CameraTransitionTL.PlayFromStart();
}

void AMainPlayer::StopLookAtTimeline(){
	CameraTransitionTL.Stop();
}

void AMainPlayer::LookAtTargetUpdate(float Value){
	MainPlayerController->SetControlRotation(UKismetMathLibrary::RLerp(CurrentControlRotation, TargetControlRotation, Value, true));
}

void AMainPlayer::StartGlitchDashFX() const{
	UPopcornFXEmitterComponent* FXToStart;
	// choisi le FX de backup si le principal est déjà utilisé
	GlitchDashFX->IsEmitterStarted() ? FXToStart = GlitchDashFXBackup : FXToStart = GlitchDashFX;

	const int PositionAIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(GlitchDashFX, "PositionA");
	const int PositionBIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(GlitchDashFX, "PositionB");

	UPopcornFXAttributeFunctions::SetAttributeAsVector(FXToStart, PositionAIndex, GetActorLocation(), true);
	UPopcornFXAttributeFunctions::SetAttributeAsVector(FXToStart, PositionBIndex, Mark->GetActorLocation(), true);

	FXToStart->StartEmitter();
}

void AMainPlayer::GlitchCameraTrace(){
	FVector BoxHalfSize = FVector::OneVector;
	BoxHalfSize *= 50;

	const TArray<AActor*> ActorsToIgnore;
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

void AMainPlayer::ReceiveGlitchUpgrade(){
	IGlitchInterface::ReceiveGlitchUpgrade();

	const int RandomEvent = FMath::RandRange(0, 1);

	switch (RandomEvent){
		case 0:
			UpdateGolds(RemovedGlitchGolds, EGoldsUpdateMethod::TakeDamages);
			break;
		case 1:

			#if WITH_EDITOR
				if(GlitchRewindTransformList.Num() == 0){
					UE_LOG(LogTemp, Warning, TEXT("Liste de position random vide"));
					return;
				}
			#endif

			SelectRandomLocation();

			FTimerHandle TimerHandle;

			GetWorldTimerManager().SetTimer(TimerHandle, this, &AMainPlayer::ResetGlitchUpgrade, GlitchUpgradeDuration, false);

			break;
	}
}

void AMainPlayer::ResetGlitchUpgrade(){
	IGlitchInterface::ResetGlitchUpgrade();

	StartRecord();
	EnableGlitchEffect(false, GlitchUpgradeDuration, 500);
}

void AMainPlayer::StartRecord(){
	GlitchRewindTransformList.Empty();

	GetWorld()->GetTimerManager().SetTimer(RewindTimer, this, &AMainPlayer::RecordRandomLocation, RewindSpacesSave, true);
}

void AMainPlayer::StopRecord(){
	GetWorld()->GetTimerManager().ClearTimer(RewindTimer);
}

void AMainPlayer::SelectRandomLocation(){
	EnableGlitchEffect(true, GlitchUpgradeDuration, 500);
	StopRecord();

	const FTransform RandomTransform = GlitchRewindTransformList[0];

	SetActorLocation(RandomTransform.GetLocation());
	Controller->SetControlRotation(RandomTransform.GetRotation().Rotator());
}

void AMainPlayer::RecordRandomLocation(){
	if(GlitchRewindTransformList.Num() == MaxRewindList){
		GlitchRewindTransformList.RemoveAt(0);
	}

	FTransform TransformToAdd;
	TransformToAdd.SetLocation(GetActorLocation());

#if !UE_BUILD_SHIPPING
	if(!IsValid(Controller)){
		return;
	}
#endif

	TransformToAdd.SetRotation(Controller->GetControlRotation().Quaternion());

	GlitchRewindTransformList.Add(TransformToAdd);
}

void AMainPlayer::EnableGlitchEffect(const bool bEnable, const float EffectDuration, const float GlitchValue){
	if(bEnable){
		FPostProcessSettings NewSettings;
		NewSettings.WeightedBlendables.Array.Add(PostProcessMaterialUI);
		GetFollowCamera()->PostProcessSettings = NewSettings;
	}

	TargetGlitchUIValue = GlitchValue;

	FadeInGlitchEffectTimeline.SetPlayRate(1/EffectDuration);

	bEnable ? FadeInGlitchEffectTimeline.Play() : FadeInGlitchEffectTimeline.Reverse();
}

void AMainPlayer::FadeInGlitchEffect(float Value){
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), GlitchMPC, "ApparitionPostProcess", FMath::Lerp(0.0f, TargetGlitchUIValue, Value));
}

void AMainPlayer::EndFadeIn(){
	const float GlitchEffectValue = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), GlitchMPC, "ApparitionPostProcess");

	if (GlitchEffectValue == 0){
		FPostProcessSettings NewSettings;
		NewSettings.WeightedBlendables.Array.Empty();
		GetFollowCamera()->PostProcessSettings = NewSettings;
	}
}

void AMainPlayer::UpdateGlitchGaugeFeedback(const float GlitchValue, const float GlitchMaxValue){
	const float PercentValue = (GlitchValue * 100) / GlitchMaxValue;
	constexpr float TierOne = 100/3;
	constexpr float TierTwo = 200/3;
	constexpr float TierThree = 300/3;

	if(PercentValue <= TierOne){
		const float GlitchParam = PercentValue/TierOne;

		SetGlitchMaterialParameter(3, GlitchParam);
		SetGlitchMaterialParameter(5, GlitchParam);
	}

	else if(PercentValue <= TierTwo){
		const float GlitchParam = (PercentValue - TierOne)/TierOne;

		SetGlitchMaterialParameter(3, 1);
		SetGlitchMaterialParameter(5, 1);

		SetGlitchMaterialParameter(2, GlitchParam);
		SetGlitchMaterialParameter(4, GlitchParam);
	}

	else if(PercentValue <= TierThree){
		const float GlitchParam = (PercentValue - TierTwo)/TierOne;

		SetGlitchMaterialParameter(2, 1);
		SetGlitchMaterialParameter(4, 1);

		SetGlitchMaterialParameter(1, GlitchParam);
		SetGlitchMaterialParameter(0, GlitchParam);
	}
}


void AMainPlayer::SetGlitchMaterialParameter(const int MaterialIndex, const float Value) const{
	UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(GetMesh()->GetMaterial(MaterialIndex));

	if (!DynamicMaterial){
		DynamicMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamic(MaterialIndex);
	}

	DynamicMaterial->SetScalarParameterValue("Apparition", Value);
}


void AMainPlayer::EndTL(){

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
		MainPlayerController->SetCanSave(true);

		ResetOverlappedMeshes();

		Mark->ResetMark();
		GetMesh()->SetVisibility(true, true);

		if(IsValid(CurrentDrone)){
			CurrentDrone->GetMesh()->SetVisibility(true, true);
		}

		GetCharacterMovement()->GravityScale = OriginalGravityScale;

		bGoldsCanBeUpdated = true;

		UUsefullFunctions::MakeNoise(this, GetActorLocation(), GlitchDashNoiseRange);
	}, 0.2f, false);
}

#pragma endregion