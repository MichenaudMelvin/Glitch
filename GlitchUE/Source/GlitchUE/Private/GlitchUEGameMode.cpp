// Copyright Epic Games, Inc. All Rights Reserved.

#include "GlitchUEGameMode.h"
#include "Player/MainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "PlacableObject/PlacableActor.h"
#include "UObject/ConstructorHelpers.h"
#include "AI/Waves/WaveManager.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "AI/MainAICharacter.h"
#include "AI/MainAIPawn.h"
#include "AI/AIPursuitDrone/PursuitDrone.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Helpers/Debug/DebugPawn.h"
#include "Curves/CurveLinearColor.h"
#include "FX/Dissolver.h"
#include "Mark/Mark.h"
#include "Objectives/Inhibiteur.h"
#include "Saves/StealthSave.h"
#include "Saves/TowerDefenseSave.h"
#include "Saves/WorldSave.h"

AGlitchUEGameMode::AGlitchUEGameMode(){
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UCurveLinearColor> Color(TEXT("/Game/Blueprint/Curves/CC_LevelStateCurve"));
	check(Color.Succeeded());

	ColorCurve = Color.Object;

	static ConstructorHelpers::FObjectFinder<UCurveLinearColor> Blinking(TEXT("/Game/Blueprint/Curves/CC_BlinkingAlertCurve"));
	check(Blinking.Succeeded());

	BlinkingCurve = Blinking.Object;
}

void AGlitchUEGameMode::BeginPlay() {
	Super::BeginPlay();

	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	TArray<AWaveManager*> WaveManagerArray;
	FindAllActors<AWaveManager>(GetWorld(), WaveManagerArray);

	TArray<AActor*> SceneCaptureArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASceneCapture2D::StaticClass(), SceneCaptureArray);

	TArray<AActor*> NexusArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANexus::StaticClass(), NexusArray);


#if WITH_EDITOR

	if (WaveManagerArray.Num() == 0){
		UE_LOG(LogTemp, Fatal, TEXT("AUCUN WAVE MANAGER N'EST PLACE DANS LA SCENE"));
	}

	if(SceneCaptureArray.Num() == 0){
		UE_LOG(LogTemp, Fatal, TEXT("AUCUN SCENE CAPTURE N'EST PLACE DANS LA SCENE"));
	}

#endif

	WaveManager = WaveManagerArray[0];

	SceneCapture = Cast<ASceneCapture2D>(SceneCaptureArray[0]);

	Nexus = Cast<ANexus>(NexusArray[0]);

	FOnTimelineLinearColor UpdateEvent;
	FOnTimelineEvent FinishEvent;

	UpdateEvent.BindDynamic(this, &AGlitchUEGameMode::UpdateLevelColor);
	FinishEvent.BindDynamic(this, &AGlitchUEGameMode::AlertLevelFinished);

	LevelStateTimeline.AddInterpLinearColor(ColorCurve, UpdateEvent);
	LevelStateTimeline.SetTimelineFinishedFunc(FinishEvent);

	FinishEvent.Unbind();
	FinishEvent.BindDynamic(this, &AGlitchUEGameMode::BlinkingFinished);

	BlinkingTimeline.AddInterpLinearColor(BlinkingCurve, UpdateEvent);
	BlinkingTimeline.SetTimelineFinishedFunc(FinishEvent);

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGlitchUEGameMode::InitializeWorld, 0.01f, false);
}

void AGlitchUEGameMode::Tick(float deltaTime){
	Super::Tick(deltaTime);

	LevelStateTimeline.TickTimeline(deltaTime);
	BlinkingTimeline.TickTimeline(deltaTime);
}

void AGlitchUEGameMode::InitializeWorld(){
	if(OptionsString == ""){
		return;
	}

	TArray<FString> LevelSettings;
	const int SettingsNbr = OptionsString.ParseIntoArray(LevelSettings, TEXT("|"), true);

	if(SettingsNbr < 2){
		return;
	}

	if(LevelSettings[0] == "?WorldSaveLoad"){
		InitializeWorldSave(LevelSettings);
	}
}

void AGlitchUEGameMode::InitializeWorldSave(TArray<FString> LevelSettings){
	const int SlotIndex = FCString::Atoi(*LevelSettings[1]);

	UWorldSave* CurrentSave = Cast<UWorldSave>(UUsefullFunctions::LoadSave(UWorldSave::StaticClass(), SlotIndex, false));

	MainPlayer->InitializePlayer(CurrentSave->PlayerTransform, CurrentSave->PlayerCameraRotation, CurrentSave->MarkTransform, CurrentSave->bIsMarkPlaced);

	AddGlitch(CurrentSave->GlitchValue);

	//Inhibiteur
	TArray<AActor*> InhibiteurList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInhibiteur::StaticClass(), InhibiteurList);

	TArray<FString> FStringArrayBis;
	CurrentSave->InhibiteurStateList.GetKeys(FStringArrayBis);

	for(int i = 0; i < InhibiteurList.Num(); i++){
		for(int j = 0; j < FStringArrayBis.Num(); j++){
			if(InhibiteurList[i]->GetName() == FStringArrayBis[j]){

				AInhibiteur* CurrentInhibiteur = Cast<AInhibiteur>(InhibiteurList[i]);

				if(CurrentSave->InhibiteurStateList.FindRef(InhibiteurList[i]->GetName())){
					CurrentInhibiteur->GetActivableComp()->ActivateObject();
				}

			}
		}
	}

	if(CurrentSave->IsA(UStealthSave::StaticClass())){
		CurrentSave = StealthWorldLoad(CurrentSave);
	} else if(CurrentSave->IsA(UTowerDefenseSave::StaticClass())){
		CurrentSave = TowerDefenseWorldLoad(CurrentSave);
	}

	CurrentSave->LoadedTime++;

	if(CurrentSave->LoadedTime >= MaxLoadSaveTime){
		UUsefullFunctions::DeleteSaveSlot(CurrentSave, SlotIndex);
		return;
	}

	OptionsString = "";
	UUsefullFunctions::SaveToSlot(CurrentSave, SlotIndex);
}

void AGlitchUEGameMode::GlobalWorldSave(const int Index){
	TSubclassOf<UWorldSave> TargetSaveClass;

	switch (CurrentPhase) {
		case EPhases::Infiltration:
			TargetSaveClass = UStealthSave::StaticClass();
			break;
		case EPhases::TowerDefense:
			TargetSaveClass = UTowerDefenseSave::StaticClass();
			break;
	}

	UWorldSave* CurrentSave = Cast<UWorldSave>(UUsefullFunctions::LoadSave(TargetSaveClass, Index, false));

	if(!CurrentSave->IsA(TargetSaveClass)){
		CurrentSave = Cast<UWorldSave>(UUsefullFunctions::CreateSave(TargetSaveClass, Index));
	}

	CurrentSave->LoadedTime = 0;

	SceneCapture->SetActorLocation(MainPlayer->GetFollowCamera()->GetComponentLocation());
	SceneCapture->SetActorRotation(MainPlayer->GetFollowCamera()->GetComponentRotation());
	SceneCapture->GetCaptureComponent2D()->CaptureScene();

	SceneCapture->GetCaptureComponent2D()->TextureTarget = SaveRenderTarget[0]; // use index

	CurrentSave->SaveImage = SaveMaterials[0]; // use index

	//Player
	CurrentSave->PlayerTransform = MainPlayer->GetActorTransform();
	CurrentSave->PlayerCameraRotation = MainPlayer->GetController()->GetControlRotation();

	if(MainPlayer->GetMark()->GetIsMarkPlaced()){
		CurrentSave->MarkTransform = MainPlayer->GetMark()->GetActorTransform();
		CurrentSave->bIsMarkPlaced = true;
	} else{
		CurrentSave->bIsMarkPlaced = false;
	}

	// Inhibiteurs
	TArray<AActor*> InhibiteurList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInhibiteur::StaticClass(), InhibiteurList);

	CurrentSave->InhibiteurStateList.Empty();
	for(int i = 0; i < InhibiteurList.Num(); i++){
		AInhibiteur* CurrentInhibiteur = Cast<AInhibiteur>(InhibiteurList[i]);

		CurrentSave->InhibiteurStateList.Add(CurrentInhibiteur->GetName(), CurrentInhibiteur->GetActivableComp()->IsActivated());
	}

	CurrentSave->GlitchValue = GlitchValue;

	switch (CurrentPhase) {
	case EPhases::Infiltration:
		CurrentSave = StealthWorldSave(CurrentSave);
		break;
	case EPhases::TowerDefense:
		CurrentSave = TowerDefenseWorldSave(CurrentSave);
		break;
	}

	UUsefullFunctions::SaveToSlot(CurrentSave, Index);
}

void AGlitchUEGameMode::GlobalWorldLoad(const int Index){
	ISaveInterface::GlobalWorldLoad(Index);

	const UWorldSave* CurrentSave = Cast<UWorldSave>(UUsefullFunctions::LoadSave(UWorldSave::StaticClass(), Index, false));

	if(CurrentSave == nullptr){
		return;
	}

	FString LoadOptions = "WorldSaveLoad|";

	LoadOptions += FString::FromInt(Index);

	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), true, LoadOptions);
}

void AGlitchUEGameMode::LaunchStealthTimer(float TimerValue){
	if(MainPlayer->GetMainPlayerController()->GetTimerWidget()->IsTimerRunning()){
		return;
	}

	if(TimerValue <= 0){
		TimerValue = StealthTimer;
	}

	FKOnFinishTimer EndEvent;
	EndEvent.BindDynamic(this, &AGlitchUEGameMode::EndStealthTimer);

	MainPlayer->GetMainPlayerController()->GetTimerWidget()->StartTimer(TimerValue, EndEvent);
}

bool AGlitchUEGameMode::CanStartTowerDefense() const{
	return CurrentActivatedCatalyseurs >= MaxCatalyseurToActivate;
}

void AGlitchUEGameMode::ForceEndStealthPhase() const{
	Nexus->GetActivableComp()->ActivateObject();
}

void AGlitchUEGameMode::UpdateActivatedCatalyseurAmount(const bool Increase){
	Increase ? CurrentActivatedCatalyseurs++ : CurrentActivatedCatalyseurs--;
}

int AGlitchUEGameMode::GetActivatedCatalyseurNum() const{
	return CurrentActivatedCatalyseurs;
}

UWorldSave* AGlitchUEGameMode::StealthWorldSave(UWorldSave* CurrentSave){
	UStealthSave* CastedSave = Cast<UStealthSave>(CurrentSave);

	TArray<AActor*> AIList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainAIController::StaticClass(), AIList);

	CastedSave->AIDataList.Empty();
	for(int i = 0; i < AIList.Num(); i++){
		AMainAIController* CurrentController = Cast<AMainAIController>(AIList[i]);

		CastedSave->AIDataList.Add(CurrentController->GetControllerName(), CurrentController->SaveAI());
	}

	CastedSave->LevelState = LevelState;

	return CastedSave;
}

UWorldSave* AGlitchUEGameMode::TowerDefenseWorldSave(UWorldSave* CurrentSave){
	UTowerDefenseSave* CastedSave = Cast<UTowerDefenseSave>(CurrentSave);

	TArray<AActor*> PlacableList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlacableActor::StaticClass(), PlacableList);

	CastedSave->PlacableDataList.Empty();
	for(int i = 0; i < PlacableList.Num(); i++){

		//the preview one should not be saved
		if(PlacableList[i]->IsA(APreviewPlacableActor::StaticClass())){
			continue;
		}

		APlacableActor* CurrentPlacable = Cast<APlacableActor>(PlacableList[i]);

		CastedSave->PlacableDataList.Add(CurrentPlacable->SavePlacable());
	}

	CastedSave->PlayerGolds = MainPlayer->GetGolds();

	CastedSave->CurrentWave = WaveManager->GetCurrentWaveNumber();

	return CastedSave;
}

UWorldSave* AGlitchUEGameMode::StealthWorldLoad(UWorldSave* CurrentSave){
	UStealthSave* CastedSave = Cast<UStealthSave>(CurrentSave);

	TArray<AActor*> AIList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainAIController::StaticClass(), AIList);

	TArray<FString> FStringArray;
	CastedSave->AIDataList.GetKeys(FStringArray);

	for(int i = 0; i < AIList.Num(); i++){
		bool bFindAI = false;

		for(int j = 0; j < FStringArray.Num(); j++){
			if(Cast<AMainAIController>(AIList[i])->GetControllerName() == FStringArray[j]){
				Cast<AMainAIController>(AIList[i])->InitializeAI(CastedSave->AIDataList.FindRef(Cast<AMainAIController>(AIList[i])->GetControllerName()));
				bFindAI = true;
				break;
			}
		}

		if(!bFindAI){
			Cast<AMainAIController>(AIList[i])->GetPawn()->Destroy();
		}
	}

	SetLevelState(CastedSave->LevelState);

	return CastedSave;
}

UWorldSave* AGlitchUEGameMode::TowerDefenseWorldLoad(UWorldSave* CurrentSave){
	UTowerDefenseSave* CastedSave = Cast<UTowerDefenseSave>(CurrentSave);

	FActorSpawnParameters PlacableSpawnParam;

	ForceEndStealthPhase();

	WaveManager->SetWave(CastedSave->CurrentWave);

	for(int i = 0; i < CastedSave->PlacableDataList.Num(); i++){
		const TSubclassOf<APlacableActor> TargetClass = CastedSave->PlacableDataList[i].PlacableActorClass;
		const FVector TargetLocation = CastedSave->PlacableDataList[i].ActorTransform.GetLocation();
		const FRotator TargetRotation = CastedSave->PlacableDataList[i].ActorTransform.Rotator();

		APlacableActor* CurrentPlacableActor = GetWorld()->SpawnActor<APlacableActor>(TargetClass, TargetLocation, TargetRotation, PlacableSpawnParam);

		CurrentPlacableActor->SetData(CastedSave->PlacableDataList[i].CurrentPlacableData);
	}

	MainPlayer->GiveGolds(CastedSave->PlayerGolds);

	return CastedSave;
}

void AGlitchUEGameMode::EndStealthTimer(){
	CanStartTowerDefense() ? ForceEndStealthPhase() : MainPlayer->GetHealthComp()->TakeMaxDamages();
}

EPhases AGlitchUEGameMode::GetPhases() const{
	return CurrentPhase;
}

void AGlitchUEGameMode::SetNewPhase(const EPhases NewPhase){
	if(CurrentPhase == NewPhase){
		return;
	}

	CurrentPhase = NewPhase;

	OnSwitchPhases.Broadcast(CurrentPhase);

	switch (CurrentPhase){
	case EPhases::Infiltration:
		break;
	case EPhases::TowerDefense:
		MainPlayer->GetMainPlayerController()->GetTimerWidget()->ForceFinishTimer(false);

		if(OptionsString == ""){
			WaveManager->StartWave();
		}

		MainPlayer->GetMainPlayerController()->SetCanSave(false);
		break;
	}
}

ELevelState AGlitchUEGameMode::GetLevelState() const{
	return LevelState;
}

void AGlitchUEGameMode::SetLevelState(const ELevelState NewState){
	if(LevelState == NewState){
		return;
	}

	LevelState = NewState;

	OnSwitchLevelState.Broadcast(LevelState);

	switch (LevelState){
	case ELevelState::Normal:
		RequestNormalState = true;
		LevelStateTimelineDirection = ETimelineDirection::Backward;
		break;
	case ELevelState::Alerted:

		LaunchStealthTimer(StealthTimer);

		LevelStateTimeline.Play();
		LevelStateTimelineDirection = ETimelineDirection::Forward;
		break;
	}
}

void AGlitchUEGameMode::UpdateLevelColor(FLinearColor NewColor){
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), AlertedMaterial, FName(TEXT("Emissive")), NewColor);
}

void AGlitchUEGameMode::AlertLevelFinished(){
	switch (LevelStateTimelineDirection) {
	case ETimelineDirection::Forward:
		BlinkingTimeline.PlayFromStart();
		BlinkingTimelineDirection = ETimelineDirection::Forward;
		break;
	case ETimelineDirection::Backward:
		break;
	}
}

void AGlitchUEGameMode::BlinkingFinished(){
	switch (BlinkingTimelineDirection){
	case ETimelineDirection::Forward:
		if (!RequestNormalState) {
			BlinkingTimeline.Reverse();
			BlinkingTimelineDirection = ETimelineDirection::Backward;
		} else {
			RequestNormalState = false;
			LevelStateTimeline.Reverse();
		}
		break;
	case ETimelineDirection::Backward:
		BlinkingTimeline.Play();
		BlinkingTimelineDirection = ETimelineDirection::Forward;
		break;
	}
}

void AGlitchUEGameMode::AddGlitch(const float AddedValue){
	GlitchValue = FMath::Clamp(AddedValue + GlitchValue, 0.0f, GlitchMaxValue);

	MainPlayer->UpdateGlitchGaugeFeedback(GlitchValue, GlitchMaxValue);

	if (GlitchValue == GlitchMaxValue) {

		CheckAvailableGlitchEvents();
		Glitch::EGlitchEvents RandomGlitchType = Glitch::EGlitchEvents::UpgradePlayer;

		switch (CurrentPhase){
			case EPhases::Infiltration:
				RandomGlitchType = static_cast<Glitch::EGlitchEvents>(FMath::RandRange(MainPlayer->GetMainPlayerController()->GetTimerWidget()->IsTimerRunning() ? 0 : Glitch::StealthIndex, Glitch::BothIndex));
				break;
			case EPhases::TowerDefense:
				RandomGlitchType = static_cast<Glitch::EGlitchEvents>(FMath::RandRange(Glitch::StealthIndex, Glitch::TowerDefenseIndex));
				break;
		}

		switch (RandomGlitchType){
			case Glitch::UpgradeAlliesUnits:
				GlitchUpgradeAlliesUnits();
				break;

			case Glitch::UpgradeEnemiesAI:
				GlitchUpgradeEnemiesAI();
				break;

			case Glitch::UpgradePlayer:
				GlitchUpgradePlayer();
				break;

			case Glitch::UpgradeWorld:
				GlitchUpgradeWorld();
				break;
		}

		GlitchValue = 0;
		MainPlayer->GetMesh()->SetScalarParameterValueOnMaterials("Apparition", 0);
	}
}

float AGlitchUEGameMode::GetCurrentGlitchValue() const{
	return GlitchValue;
}

void AGlitchUEGameMode::GlitchUpgradeAlliesUnits() const{
	TArray<AActor*> PlacableActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlacableActor::StaticClass(), PlacableActorList);

	PlacableActorList = UUsefullFunctions::SortActorsByDistanceToActor(PlacableActorList, MainPlayer);

	int Count = 0;

	for(int i = 0; i < PlacableActorList.Num(); i++){
		if(PlacableActorList[i]->IsA(APreviewPlacableActor::StaticClass())){
			continue;
		}

		if(PlacableActorList[i]->IsA(APursuitDrone::StaticClass()) && CurrentPhase == EPhases::TowerDefense){
			continue;
		}

		Count++;
		Cast<IGlitchInterface>(PlacableActorList[i])->ReceiveGlitchUpgrade();

		if(Count == NumberOfAlliesUnitsToAffect){
			return;
		}
	}
}

void AGlitchUEGameMode::GlitchUpgradeEnemiesAI() const{
	TArray<AActor*> AIControllerList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainAIController::StaticClass(), AIControllerList);

	TArray<AActor*> AIList;
	for(int i = 0; i < AIControllerList.Num(); i++){
		AIList.Add(Cast<AMainAIController>(AIControllerList[i])->GetPawn());
	}

	if (AIList.Num() > 0) {
		AIList = UUsefullFunctions::SortActorsByDistanceToActor(AIList, MainPlayer);

		const int LoopNumber = AIList.Num() < NumberOfEnemiesToAffect ? AIList.Num() : NumberOfEnemiesToAffect;

		for(int i = 0; i < LoopNumber; i++){
			Cast<IGlitchInterface>(AIList[i])->ReceiveGlitchUpgrade();
		}
	}
}

void AGlitchUEGameMode::GlitchUpgradePlayer() const{
	Cast<IGlitchInterface>(MainPlayer)->ReceiveGlitchUpgrade();
}

void AGlitchUEGameMode::GlitchUpgradeWorld() const{
	const float CurrentTime = MainPlayer->GetMainPlayerController()->GetTimerWidget()->GetTimerElapsed();

	const float NewTime = FMath::Clamp(CurrentTime - GlitchReduceStealthTimer, 1.0f, StealthTimer);

	MainPlayer->GetMainPlayerController()->GetTimerWidget()->ChangeTimerValue(NewTime);
}

void AGlitchUEGameMode::CheckAvailableGlitchEvents() const{
	TArray<AActor*> PlacableActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlacableActor::StaticClass(), PlacableActorList);

	TArray<AActor*> AIList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainAICharacter::StaticClass(), AIList);

}

#pragma region ConsoleCommands

void AGlitchUEGameMode::SetSelfTimeDilation(float TimeDilation) const{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn()->CustomTimeDilation = TimeDilation;
}

void AGlitchUEGameMode::NextWave() const{
	WaveManager->NextWave();
}

void AGlitchUEGameMode::GoToWave(const int NewWave) const{
	WaveManager->SetWave(NewWave);
}

void AGlitchUEGameMode::ToggleSpectatorMode(const bool bToggleAtLocation) const{
	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADebugPawn::StaticClass(), ActorList);

	if(ActorList.Num() == 0){
		const FActorSpawnParameters SpawnInfo;
		ADebugPawn* SpawnedPawn = GetWorld()->SpawnActor<ADebugPawn>(MainPlayer->GetActorLocation(), MainPlayer->GetActorRotation(), SpawnInfo);
		MainPlayer->Controller->Possess(SpawnedPawn);
	} else{
		if(bToggleAtLocation){
			MainPlayer->SetActorLocation(ActorList[0]->GetActorLocation());

			FRotator PlayerRotation = Cast<APawn>(ActorList[0])->Controller->GetControlRotation();
			PlayerRotation.Pitch = 0;

			MainPlayer->SetActorRotation(PlayerRotation);
		}

		Cast<APawn>(ActorList[0])->Controller->Possess(MainPlayer);
		MainPlayer->GetMainPlayerController()->SelectNewGameplayMode(MainPlayer->GetMainPlayerController()->GetGameplayMode());

		ActorList[0]->Destroy();
	}
}

void AGlitchUEGameMode::Dissolve(const float Value) const{
	TArray<AActor*> DissolverArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADissolver::StaticClass(), DissolverArray);

	Cast<ADissolver>(DissolverArray[0])->DissolveTo(Value);
}

#pragma endregion
