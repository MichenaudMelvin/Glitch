// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/MainPlayer.h"
#include "FMODBlueprintStatics.h"
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
#include "Gamemodes/GlitchUEGameMode.h"
#include "NavigationSystem.h"
#include "PopcornFXAttributeFunctions.h"
#include "PopcornFXEmitterComponent.h"
#include "PopcornFXFunctions.h"
#include "AI/MainAICharacter.h"
#include "Helpers/FunctionsLibrary/UsefulFunctions.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Player/MainPlayerController.h"
#include "Mark/GlitchMark.h"
#include "Components/CompassComponent.h"
#include "AI/AIPursuitDrone/PursuitDrone.h"
#include "PlacableObject/ConstructionZone.h"

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

	HearingTrigger = CreateDefaultSubobject<UHearingTriggerComponent>(TEXT("Hearing Trigger"));

	#pragma region Timelines

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_ZeroToOneCurve"));
	check(Curve.Succeeded());

	ZeroToOneCurve = Curve.Object;

	static ConstructorHelpers::FObjectFinder<UCurveFloat> SmoothCurve(TEXT("/Game/Blueprint/Curves/FC_SmoothZeroToOneCurve"));
	check(SmoothCurve.Succeeded());

	SmoothZeroToOneCurve = SmoothCurve.Object;

	#pragma endregion

	SoundFX = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("Run FX"));

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> RunEffect(TEXT("/Game/VFX/Particles/FX_Avatar/Pk_SoundDetection"));
	check(RunEffect.Succeeded());

	SoundFX->SetEffect(RunEffect.Object);
	SoundFX->SetupAttachment(GetMesh());
	SoundFX->bPlayOnLoad = false;

	LoseGoldsFX = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("Lose Gold FX"));

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> GoldEffect(TEXT("/Game/VFX/Particles/FX_Avatar/Pk_LoseGold"));
	check(GoldEffect.Succeeded());

	LoseGoldsFX->SetEffect(GoldEffect.Object);
	LoseGoldsFX->SetupAttachment(GetMesh());
	LoseGoldsFX->SetRelativeLocation(FVector(0, 0, 80));
	LoseGoldsFX->bPlayOnLoad = false;

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> FX(TEXT("/Game/VFX/Particles/FX_Avatar/Pk_TPDash"));
	check(FX.Succeeded());

	GlitchDashFXReference = FX.Object;

	static ConstructorHelpers::FObjectFinder<UFMODEvent> StartSFX(TEXT("/Game/FMOD/Events/SFX/SFX_glitch_dash_Start"));
	check(StartSFX.Succeeded());

	TPStart = StartSFX.Object;

	static ConstructorHelpers::FObjectFinder<UFMODEvent> EndSFX(TEXT("/Game/FMOD/Events/SFX/SFX_glitch_dash_End"));
	check(EndSFX.Succeeded());

	TPEnd = EndSFX.Object;
}

void AMainPlayer::BeginPlay(){
	Super::BeginPlay();

	GameplaySettingsSaveSave = Cast<UGameplaySettingsSave>(UUsefulFunctions::LoadSave(UGameplaySettingsSave::StaticClass(), 0));
	FollowCamera->FieldOfView = GameplaySettingsSaveSave->CameraFOV;
	Sensitivity = GameplaySettingsSaveSave->CameraSensitivity;
	bInvertYAxis = GameplaySettingsSaveSave->bInvertCamYAxis;

	MainPlayerController = Cast<AMainPlayerController>(GetController());

	GameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->OnSwitchPhases.AddDynamic(this, &AMainPlayer::OnSwitchPhases);

	WheelCamera = Cast<ATargetCameraLocation>(UGameplayStatics::GetActorOfClass(GetWorld(), ATargetCameraLocation::StaticClass()));

#if WITH_EDITOR
	if(!IsValid(WheelCamera)){
		GEngine->AddOnScreenDebugMessage(-1, 9999999999.0f, FColor::Yellow, TEXT("AUCUNE TARGETCAMERALOCATION N'EST PLACE DANS LA SCENE"));
		UE_LOG(LogTemp, Warning, TEXT("AUCUNE TARGETCAMERALOCATION N'EST PLACE DANS LA SCENE"));
	}
#endif

	HalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	FOnTimelineFloat UpdateEvent;
	FOnTimelineEvent FinishedEvent;

	UpdateEvent.BindDynamic(this, &AMainPlayer::LookAtTargetUpdate);

	CameraTransitionTL.AddInterpFloat(ZeroToOneCurve, UpdateEvent);

	UpdateEvent.Unbind();
	FinishedEvent.Unbind();
	UpdateEvent.BindDynamic(this, &AMainPlayer::CameraAimUpdate);
	FinishedEvent.BindDynamic(this, &AMainPlayer::CameraAimFinished);

	CameraAimTransition.AddInterpFloat(bUseSmoothAim ? SmoothZeroToOneCurve : ZeroToOneCurve, UpdateEvent);
	CameraAimTransition.SetTimelineFinishedFunc(FinishedEvent);
	CameraAimTransition.SetPlayRate(1/CameraAimTime);

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

	UpdateEvent.Unbind();
	FinishedEvent.Unbind();

	UpdateEvent.BindDynamic(this, &AMainPlayer::AppearUpdate);
	FinishedEvent.BindDynamic(this, &AMainPlayer::EndAppear);

	AppearTimeline.AddInterpFloat(ZeroToOneCurve, UpdateEvent);
	AppearTimeline.SetTimelineFinishedFunc(FinishedEvent);
	AppearTimeline.SetPlayRate(1/AppearTime);

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

	GlitchDashFX = UPopcornFXFunctions::SpawnEmitterAtLocation(GetWorld(), GlitchDashFXReference, "PopcornFX_DefaultScene", FVector::ZeroVector, FRotator::ZeroRotator, false, false);
	GlitchDashFXBackup = UPopcornFXFunctions::SpawnEmitterAtLocation(GetWorld(), GlitchDashFXReference, "PopcornFX_DefaultScene", FVector::ZeroVector, FRotator::ZeroRotator, false, false);

	const int TargetIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(GlitchDashFX, "TeleportLifetime");

	UPopcornFXAttributeFunctions::SetAttributeAsFloat(GlitchDashFX, TargetIndex, GlitchDashDuration + 1, true);
	UPopcornFXAttributeFunctions::SetAttributeAsFloat(GlitchDashFXBackup, TargetIndex, GlitchDashDuration + 1, true);

	#pragma endregion

	FWorldDelegates::OnWorldCleanup.AddUFunction(this, "OnCleanWorld");

#if WITH_EDITORONLY_DATA
	if(!bPlayStartAnim){
		EndAppear();
		return;
	}
#endif

	MainPlayerController->BindPause();
	MakeThePlayerAppear();
}

void AMainPlayer::Destroyed(){
	GetWorld()->GetTimerManager().ClearTimer(RewindTimer);
	GetWorld()->GetTimerManager().ClearTimer(CanLaunchMarkTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(CoyoteTimeTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(BackToNormalCamTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(GlitchUpgradeTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(GlitchDashTimerHandle);

	Super::Destroyed();
}

void AMainPlayer::OnCleanWorld(UWorld* World, bool bSessionEnded, bool bCleanupResources){
	World->GetTimerManager().ClearTimer(RewindTimer);
	World->GetTimerManager().ClearTimer(CanLaunchMarkTimerHandle);
	World->GetTimerManager().ClearTimer(CoyoteTimeTimerHandle);
	World->GetTimerManager().ClearTimer(BackToNormalCamTimerHandle);
	World->GetTimerManager().ClearTimer(GlitchUpgradeTimerHandle);
	World->GetTimerManager().ClearTimer(GlitchDashTimerHandle);
}

void AMainPlayer::InitializePlayer_Implementation(const FTransform StartTransform, const FRotator CameraRotation, const FTransform MarkTransform, const bool bIsMarkPlaced){
	SetActorTransform(StartTransform);
	Controller->SetControlRotation(CameraRotation);

	if(bIsMarkPlaced){
		Mark->SetActorTransform(MarkTransform);
		Mark->PlaceMark();
	}
}

#pragma region Camera

void AMainPlayer::CameraAim(){
	if(!MainPlayerController->GetSightWidget()->IsInViewport()){
		MainPlayerController->GetSightWidget()->AddToViewport();
	}

	CameraAimTransition.Play();
	CameraAimTimelineDirection = ETimelineDirection::Forward;
}

void AMainPlayer::CameraAimReverse(){
	CameraAimTransition.Reverse();
	CameraAimTimelineDirection = ETimelineDirection::Backward;
}

void AMainPlayer::CameraStopAim(){
	CameraAimTransition.Stop();
}

void AMainPlayer::CameraAimUpdate(float Alpha){
	CameraBoom->SocketOffset = UKismetMathLibrary::VLerp(FVector::ZeroVector, AimOffset, Alpha);
	MainPlayerController->GetSightWidget()->UpdateOpacity(Alpha);
}

void AMainPlayer::CameraAimFinished(){
	switch (CameraAimTimelineDirection){
	case ETimelineDirection::Forward:
			GetWorld()->GetTimerManager().SetTimer(CanLaunchMarkTimerHandle, this, &AMainPlayer::CanLaunchMark, 0.1f, true);
			break;
		case ETimelineDirection::Backward:
			MainPlayerController->GetSightWidget()->RemoveFromParent();
			break;
	}
}

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

void AMainPlayer::PlacePlacableActor(){
	if(!PreviewPlacableActor->CanBePlaced()){
		return;
	}

	UpdateGolds(CurrentPlacableActorData->Cost, EGoldsUpdateMethod::BuyPlacable);
	APlacableActor* NewPlacable = GetWorld()->SpawnActor<APlacableActor>(CurrentPlacableActorData->ClassToSpawn, TargetPreviewActorLocation, FRotator::ZeroRotator, FActorSpawnParameters());
	NewPlacable->SetMissingData(Nexus, this);
	NewPlacable->SetData(CurrentPlacableActorData);
	CurrentFocusedConstructionZone->OccupiedSlot(NewPlacable);
}

AConstructionZone* AMainPlayer::GetCurrentConstructionZone() const{
	return CurrentFocusedConstructionZone;
}

void AMainPlayer::OpenConstructionZone(AConstructionZone* CurrentConstructionZone){
	CurrentFocusedConstructionZone = CurrentConstructionZone;
	TargetPreviewActorLocation = CurrentConstructionZone->GetActorLocation();
	PreviewPlacableActor->SetActorLocation(TargetPreviewActorLocation);

	if(CurrentFocusedConstructionZone->IsSlotOccupied()){
		PreviewPlacableActor->SetRangeMesh(CurrentFocusedConstructionZone->GetUnit()->GetData());
		PreviewPlacableActor->SetShouldRangeUpdate(false);
	}

}

APreviewPlacableActor* AMainPlayer::GetPreviewPlacableActor() const{
	return PreviewPlacableActor;
}

void AMainPlayer::SetPlacableActorData(UPlacableActorData* Data){
	if(IsValid(Data)){
		CurrentPlacableActorData = Data;
		PreviewPlacableActor->SetData(CurrentPlacableActorData);
		PreviewPlacableActor->ChooseColor();
	} else{
		PreviewPlacableActor->SetData(nullptr);
	}
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
			Amount = - Amount;
			break;
		case EGoldsUpdateMethod::TakeDamages:
			Amount = - Amount;
			LoseGoldsFX->StartEmitter();
			break;
		case EGoldsUpdateMethod::ReceiveGolds:
			break;
	}
	

	Golds += Amount;
	OnUpdateGolds.Broadcast(Golds);

	if(Golds < 0){
		Loose();
	}
}

void AMainPlayer::SetGolds(const int Amount){
	Golds = Amount;
	OnUpdateGolds.Broadcast(Golds);
}

void AMainPlayer::KillPlayer(){
	UpdateGolds(Golds + 1, EGoldsUpdateMethod::TakeDamages);
}

bool AMainPlayer::CanUpdateGolds() const{
	return bGoldsCanBeUpdated;
}

void AMainPlayer::Loose(){
	StopRecord();
	MainPlayerController->GetLooseScreen()->AddToViewport();
}

void AMainPlayer::Win(){
	StopRecord();
	MainPlayerController->GetWinScreen()->AddToViewport();
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

	if(!InteractionLineTrace(HitResult)) {
		UnfeedbackCurrentCheckedObject();
		return;
	}

	if(HitResult.Actor == nullptr) {
		UnfeedbackCurrentCheckedObject();
		return;
	}

	UInteractableComponent* HittedInteractable = Cast<UInteractableComponent>(HitResult.Actor->GetComponentByClass(UInteractableComponent::StaticClass()));

	if(!IsValid(HittedInteractable)) {
		UnfeedbackCurrentCheckedObject();
		return;
	}

	if ((HittedInteractable != CurrentCheckedObject) && (HittedInteractable->CheckComponent(HitResult.GetComponent()))){
		UnfeedbackCurrentCheckedObject();
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

	CurrentDrone->AttachDrone(this, "Head");
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

void AMainPlayer::DetachFromEdge_Implementation(){
	GetCharacterMovement()->GravityScale = OriginalGravityScale;
	MainPlayerController->BindNormalMode();
}

void AMainPlayer::TurnAtRate(const float Rate){
	// calculate delta for this frame from the rate 
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() * Sensitivity);
}

void AMainPlayer::LookUpAtRate(const float Rate){
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() * Sensitivity);
}

void AMainPlayer::Jump(){
	if(!CanStandUp()){
		return;
	}

	Super::Jump();

	MainPlayerController->SetCanSave(false);
	MainPlayerController->UnbindSneak();

	if(bUseCoyoteTime){
		bUseCoyoteTime = false;
		GetCharacterMovement()->Velocity.Z = FMath::Max(GetCharacterMovement()->Velocity.Z, GetCharacterMovement()->JumpZVelocity);
	}
}

void AMainPlayer::OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal, const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta){
	Super::OnWalkingOffLedge_Implementation(PreviousFloorImpactNormal, PreviousFloorContactNormal, PreviousLocation,TimeDelta);

	bUseCoyoteTime = true;
	MainPlayerController->SetCanSave(false);
	MainPlayerController->UnbindSneak();

	GetWorld()->GetTimerManager().SetTimer(CoyoteTimeTimerHandle, [&](){
		bUseCoyoteTime = false;
	}, CoyoteTime, false);
}

void AMainPlayer::AddControllerYawInput(float Val){
	Super::AddControllerYawInput(Val * Sensitivity);
}

void AMainPlayer::AddControllerPitchInput(float Rate){
	if (bInvertYAxis) {
		Rate = Rate * -1;
	}

	Super::AddControllerPitchInput(Rate * Sensitivity);
}

void AMainPlayer::Dash_Implementation(){}

void AMainPlayer::SprintRelease_Implementation(){}

void AMainPlayer::SneakPressed_Implementation(){}

void AMainPlayer::SneakReleased_Implementation(){}

void AMainPlayer::SprintToSneak_Implementation(){}

void AMainPlayer::SneakToSprint_Implementation(){}

void AMainPlayer::ResetMovement_Implementation(){}

bool AMainPlayer::CanStandUp(){
	FVector TraceLocation = GetMesh()->GetComponentLocation();
	TraceLocation.Z += HalfHeight;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(Mark);

	FHitResult HitResult;
	return !UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), TraceLocation, TraceLocation, GetCapsuleComponent()->GetUnscaledCapsuleRadius(), HalfHeight, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
}

void AMainPlayer::Landed(const FHitResult& Hit){
	Super::Landed(Hit);

	MainPlayerController->SetCanSave(true);
	MainPlayerController->BindSneak();

	const float FallingVelocity = GetCharacterMovement()->Velocity.Z;
	HearingTrigger->MakeNoise(this, GetActorLocation(), FMath::Abs(FallingVelocity / JumpNoiseRangeFactor), SoundFX);
}

void AMainPlayer::OnSwitchPhases(EPhases NewPhase){
	if(NewPhase == EPhases::TowerDefense){

		const float RemainTime = GetMainPlayerController()->GetTimerWidget()->IsTimerRunning() ? GetMainPlayerController()->GetTimerWidget()->GetTimerElapsed() : GameMode->GetStealthTimer();

		if(GameMode->OptionsString == ""){
			UpdateGolds(RemainTime * GameMode->GetGoldTimerMultiplier(), EGoldsUpdateMethod::ReceiveGolds);
		}

#if !UE_BUILD_SHIPPING
		// for launch game
		else if(GameMode->OptionsString == "?Name=Player"){
			UpdateGolds(RemainTime * GameMode->GetGoldTimerMultiplier(), EGoldsUpdateMethod::ReceiveGolds);
		}
#endif

		GetMainPlayerController()->GetTimerWidget()->ForceFinishTimer(false, false);
		GetMainPlayerController()->SetCanSave(false);

		SoundFX->DestroyComponent();
	}
}

void AMainPlayer::MakeMovementNoise(const float InputRate){
	if(!UUsefulFunctions::IsCharacterMovingOnGround(this)){
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

	HearingTrigger->MakeNoise(this, GetActorLocation(), NoiseRadius * FMath::Abs(InputRate), SoundFX);
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
		MakeMovementNoise(Value);
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
		MakeMovementNoise(Value);
	}
}

void AMainPlayer::ClingUpDirection(float AxisValue){
	switch (FMath::TruncToInt(AxisValue)){
		case -1:
			DetachFromEdge();
			break;
	}
}

void AMainPlayer::ClingRight(float AxisValue){
	switch (FMath::TruncToInt(AxisValue)){
		case -1:
			HorizontalCling(EDirection::Left);
			break;
		case 1:
			HorizontalCling(EDirection::Right);
			break;
	}
}

void AMainPlayer::HorizontalCling_Implementation(const EDirection Direction){}

void AMainPlayer::ClingUp_Implementation(){}

void AMainPlayer::SetInvertAxis(const bool bNewValue){
	bInvertYAxis = bNewValue;
}

void AMainPlayer::SetSensitivity(const float NewSensitivity){
	Sensitivity = NewSensitivity;
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

ATargetCameraLocation* AMainPlayer::GetWheelCamera() const{
	return WheelCamera;
}

void AMainPlayer::LaunchMark(){
	Mark->GetFakeMark()->SetActorLocation(FollowCamera->GetComponentLocation());
	Mark->GetFakeMark()->Launch(FindMarkLaunchRotation());

	MainPlayerController->UnbindGlitch();
	MainPlayerController->OnUseGlitchPressed.AddDynamic(this, &AMainPlayer::TPToMark);
}

FRotator AMainPlayer::FindMarkLaunchRotation() const{
	FHitResult HitResult;
	const FVector StartLocation = Mark->GetFakeMark()->GetActorLocation();
	const FVector EndLocation = (FollowCamera->GetForwardVector() * Mark->GetMaxLaunchDistance()) + StartLocation;

	FCollisionQueryParams QueryParams;
	FCollisionResponseParams ResponseParam;

	FVector TargetLocation = EndLocation;

	if(GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams, ResponseParam)){
		TargetLocation = HitResult.ImpactPoint;
	}

	Mark->GetFakeMark()->SetTargetPosition(TargetLocation);

	return UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
}

void AMainPlayer::TPToMark() {

	MainPlayerController->UnbindMovement();
	MainPlayerController->UnbindCamera();
	MainPlayerController->UnbindGlitch();
	MainPlayerController->SetCanSave(false);
	bCanBeAttachedToEdge = false;

	StopJumping();
	Mark->PlaceMark();
	CameraAimReverse();

	bGoldsCanBeUpdated = false;
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	// Play sound start of the teleportation
	UFMODBlueprintStatics::PlayEvent2D(GetWorld(), TPStart, true);

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

void AMainPlayer::CanLaunchMark(){
	if(!UUsefulFunctions::IsEventActionPressed("UseGlitch", MainPlayerController)){
		GetWorldTimerManager().ClearTimer(CanLaunchMarkTimerHandle);
		LaunchMark();

		GetWorld()->GetTimerManager().SetTimer(BackToNormalCamTimerHandle, this, &AMainPlayer::CameraAimReverse, BackToNormalCamAfterLaunchMarkTime, false);
	}
}

void AMainPlayer::ForceStopSneak_Implementation(){}

void AMainPlayer::Tick(float deltaTime){
	Super::Tick(deltaTime);

	CameraTransitionTL.TickTimeline(deltaTime);
	CameraAimTransition.TickTimeline(deltaTime);
	CameraZoomTransition.TickTimeline(deltaTime);
	CameraFOVTransition.TickTimeline(deltaTime);
	FadeInGlitchEffectTimeline.TickTimeline(deltaTime);
	AppearTimeline.TickTimeline(deltaTime);
}

void AMainPlayer::SetMark(AGlitchMark* NewMark){
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

void AMainPlayer::FinishGlitchDash_Implementation(){}

void AMainPlayer::ReceiveGlitchUpgrade(){
	IGlitchInterface::ReceiveGlitchUpgrade();

	#if WITH_EDITOR
		if(GlitchRewindTransformList.Num() == 0){
			UE_LOG(LogTemp, Warning, TEXT("Liste de position random vide"));
			return;
		}
	#endif

	SelectRandomLocation();

	GetWorldTimerManager().SetTimer(GlitchUpgradeTimerHandle, this, &AMainPlayer::ResetGlitchUpgrade, GlitchUpgradeDuration, false);
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
	EnablePostProcessUIEffect(bEnable, GlitchPostProcessMaterialUI, EffectDuration, GlitchValue);
}

void AMainPlayer::EnableSafeEffect(const bool bEnable, const float EffectDuration, const float SafeValue){
	EnablePostProcessUIEffect(bEnable, SafePostProcessMaterialUI, EffectDuration, SafeValue);
}

void AMainPlayer::FadeInGlitchEffect(float Value){
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), GlitchMPC, "ApparitionPostProcess", FMath::Lerp(0.0f, TargetPostProcessUIValue, Value));
}

void AMainPlayer::EnablePostProcessUIEffect(const bool bEnable, FWeightedBlendable PostProcessEffect, const float EffectDuration, const float PostProcessValue){
	if(bEnable){
		FPostProcessSettings NewSettings;
		NewSettings.WeightedBlendables.Array.Add(PostProcessEffect);
		GetFollowCamera()->PostProcessSettings = NewSettings;
	}

	TargetPostProcessUIValue = PostProcessValue;

	FadeInGlitchEffectTimeline.SetPlayRate(1/EffectDuration);

	bEnable ? FadeInGlitchEffectTimeline.Play() : FadeInGlitchEffectTimeline.Reverse();
}


void AMainPlayer::EndFadeIn(){
	const float GlitchEffectValue = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), GlitchMPC, "ApparitionPostProcess");

	if (GlitchEffectValue == 0){
		FPostProcessSettings NewSettings;
		NewSettings.WeightedBlendables.Array.Empty();
		GetFollowCamera()->PostProcessSettings = NewSettings;
	}
}

void AMainPlayer::MakeThePlayerAppear(){
	GetMesh()->SetScalarParameterValueOnMaterials("PercentageApparition", 0);
	bIsAppearing = true;

	AppearTimeline.Play();
}

void AMainPlayer::AppearUpdate(float Value){
	GetMesh()->SetScalarParameterValueOnMaterials("PercentageApparition", Value);
}

void AMainPlayer::EndAppear(){
	bIsAppearing = false;
	MainPlayerController->BindNormalMode();

	OnEndAppear.Broadcast();

	for(int i = 0; i < RealPlayerMaterialList.Num(); i++){
		GetMesh()->SetMaterial(i, RealPlayerMaterialList[i]);
	}
}

bool AMainPlayer::IsAppearing() const{
	return bIsAppearing;
}

void AMainPlayer::UpdateGlitchGaugeFeedback(const float GlitchValue, const float GlitchMaxValue) const{
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
	ForceStopSneak();

	// Play sound final of the teleportation
	UFMODBlueprintStatics::PlayEvent2D(GetWorld(), TPEnd, true);

	GlitchCameraTrace();

	GlitchTrace();

	ResetMovement();

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;

	UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), Mark->GetTPLocation(), GetCapsuleComponent()->GetRelativeRotation(), false, false, GlitchDashDuration, false, EMoveComponentAction::Type::Move, LatentInfo);

	GetWorld()->GetTimerManager().SetTimer(GlitchDashTimerHandle, [&](){
		MainPlayerController->BindNormalMode();
		GetMesh()->SetVisibility(true, true);
		Mark->ResetMark();
		MainPlayerController->SetCanSave(true);
		bCanBeAttachedToEdge = true;
		bGoldsCanBeUpdated = true;

		GetCharacterMovement()->GravityScale = OriginalGravityScale;

		GameMode->AddGlitch(GlitchDashValue + OverlappedMeshes.Num() + OverlappedAICharacters.Num());

		ResetOverlappedMeshes();

		FinishGlitchDash();

		if(IsValid(CurrentDrone)){
			CurrentDrone->GetMesh()->SetVisibility(true, true);
		}

		HearingTrigger->MakeNoise(this, GetActorLocation(), GlitchDashNoiseRange, SoundFX);
	}, GlitchDashDuration, false);
}

#pragma endregion