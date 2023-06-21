// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gamemodes/GlitchUEGameMode.h"
#include "Player/MainPlayer.h"
#include "Player/MainPlayerController.h"
#include "UI/Gameplay/PlayerStats.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "PlacableObject/PlacableActor.h"
#include "UObject/ConstructorHelpers.h"
#include "AI/Waves/WaveManager.h"
#include "Helpers/FunctionsLibrary/UsefulFunctions.h"
#include "AI/MainAICharacter.h"
#include "AI/MainAIPawn.h"
#include "AI/AIPursuitDrone/PursuitDrone.h"
#include "Audio/AudioManager.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Helpers/Debug/DebugPawn.h"
#include "Curves/CurveLinearColor.h"
#include "FX/Dissolver.h"
#include "LevelElements/BanLog.h"
#include "LevelElements/BasicDoor.h"
#include "Mark/GlitchMark.h"
#include "Objectives/Inhibiteur.h"
#include "Saves/StealthSave.h"
#include "Saves/TowerDefenseSave.h"
#include "Saves/WorldSave.h"
#include "UI/Gameplay/Tchat/TchatTriggerBox.h"

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
	MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	TArray<AWaveManager*> WaveManagerArray;
	FindAllActors<AWaveManager>(GetWorld(), WaveManagerArray);

	TArray<AActor*> NexusArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANexus::StaticClass(), NexusArray);

	TArray<AActor*> DissolverArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADissolver::StaticClass(), DissolverArray);

	TArray<AActor*> CatalyseurList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACatalyseur::StaticClass(), CatalyseurList);

	CatalyseurNumberInLevel = CatalyseurList.Num();

#if WITH_EDITOR
	if (WaveManagerArray.Num() == 0){
		UE_LOG(LogTemp, Fatal, TEXT("AUCUN WAVE MANAGER N'EST PLACE DANS LA SCENE"));
	}
#endif

	WaveManager = WaveManagerArray[0];

	Nexus = Cast<ANexus>(NexusArray[0]);

	Dissolver = Cast<ADissolver>(DissolverArray[0]);

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

	FWorldDelegates::OnWorldCleanup.AddUFunction(this, "OnCleanWorld");

	FTimerHandle TimerHandle;

	#if WITH_EDITOR
		if(!IsValid(MainPlayer) || !IsValid(MainPlayerController)){
			return;
		}
	#endif

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGlitchUEGameMode::InitializeWorld, 0.01f, false);
}

void AGlitchUEGameMode::Tick(float deltaTime){
	Super::Tick(deltaTime);

	LevelStateTimeline.TickTimeline(deltaTime);
	BlinkingTimeline.TickTimeline(deltaTime);
}

void AGlitchUEGameMode::OnCleanWorld(UWorld* World, bool bSessionEnded, bool bCleanupResources){
	World->GetTimerManager().ClearTimer(DissolveEndingTimerHandle);
	World->GetTimerManager().ClearTimer(GlitchEffectEndingTimerHandle);
}

void AGlitchUEGameMode::InitializeWorld(){
	PlayerStatsWidget = MainPlayerController->GetPlayerStatsWidget();
	UpdatePlayerObjectives();

	if(OptionsString == ""){
		GlobalWorldSave(0);
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

	UWorldSave* CurrentSave = Cast<UWorldSave>(UUsefulFunctions::LoadSave(UWorldSave::StaticClass(), SlotIndex, false));

	MainPlayer->InitializePlayer(CurrentSave->PlayerTransform, CurrentSave->PlayerCameraRotation, CurrentSave->MarkTransform, CurrentSave->bIsMarkPlaced);
	MainPlayer->SetGolds(CurrentSave->PlayerGolds);

	AddGlitch(CurrentSave->GlitchValue);

	for(int i = 0; i < CurrentSave->TchatLinesList.Num(); i++){
		MainPlayerController->GetTchatWidget()->AddTchatLineWithATchatStruct(CurrentSave->TchatLinesList[i]);
	}

	// Doors
	TArray<AActor*> DoorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasicDoor::StaticClass(), DoorList);

	TArray<FString> FStringArray;
	CurrentSave->DoorDataList.GetKeys(FStringArray);

	for(int i = 0; i < DoorList.Num(); i++){
		for(int j = 0; j < FStringArray.Num(); j++){
			if(DoorList[i]->GetName() == FStringArray[j]){

				ABasicDoor* CurrentDoor = Cast<ABasicDoor>(DoorList[i]);

				CurrentDoor->InitializeDoor(CurrentSave->DoorDataList.FindRef(DoorList[i]->GetName()));
			}
		}
	}

	// Tchat Box
	TArray<AActor*> TchatBox;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATchatTriggerBox::StaticClass(), TchatBox);

	FStringArray = CurrentSave->TchatTriggerNameList;

	for(int i = 0; i < TchatBox.Num(); i++){
		bool bFindTchatBox = false;

		for(int j = 0; j < FStringArray.Num(); j++){
			if(TchatBox[i]->GetName() == FStringArray[j]){
				bFindTchatBox = true;
				break;
			}
		}

		if(!bFindTchatBox){
			TchatBox[i]->Destroy();
		}
	}

	// Ban logs
	TArray<AActor*> BanLogList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABanLog::StaticClass(), BanLogList);

	FStringArray.Empty();
	CurrentSave->BanLogList.GetKeys(FStringArray);

	for(int i = 0; i < BanLogList.Num(); i++){
		for(int j = 0; j < FStringArray.Num(); j++){
			if(BanLogList[i]->GetName() == FStringArray[j]){
				if(CurrentSave->BanLogList.FindRef(FStringArray[j])){
					Cast<ABanLog>(BanLogList[i])->RemoveInteraction();
				}
			}
		}
	}

	if(CurrentSave->IsA(UStealthSave::StaticClass())){
		CurrentSave = StealthWorldLoad(CurrentSave);
	} else if(CurrentSave->IsA(UTowerDefenseSave::StaticClass())){
		CurrentSave = TowerDefenseWorldLoad(CurrentSave);
	}

	OptionsString = "";
	UUsefulFunctions::SaveToSlot(CurrentSave, SlotIndex);
}

void AGlitchUEGameMode::UpdatePlayerObjectives() const{
	if(!bUseAutoObjectivesForPlayer){
		return;
	}

	const int RemainingCatalyseurs = FMath::Clamp(CatalyseurNumberInLevel - CurrentActivatedCatalyseurs, 0, CatalyseurNumberInLevel);
	const int RemainingCatalyseursToActivate = FMath::Clamp(MaxCatalyseurToActivate - CurrentActivatedCatalyseurs, 0, MaxCatalyseurToActivate);

	PlayerStatsWidget->UpdateObjectivesText(RemainingCatalyseurs > 0 ? "There are " + FString::FromInt(RemainingCatalyseurs) + " generators left in this level" : ObjectiveEndMessage);
	PlayerStatsWidget->UpdateAdditionalText(RemainingCatalyseursToActivate > 0 ? "You need " + FString::FromInt(RemainingCatalyseursToActivate) + " generators to activate the Nexus" : AdditionalStealthEndMessage);
}

void AGlitchUEGameMode::GlobalWorldSave(const int Index){
	if(OptionsString != ""){
		return;
	}

	TSubclassOf<UWorldSave> TargetSaveClass;

	switch (CurrentPhase) {
		case EPhases::Infiltration:
			TargetSaveClass = UStealthSave::StaticClass();
			break;
		case EPhases::TowerDefense:
			TargetSaveClass = UTowerDefenseSave::StaticClass();
			break;
	}

	UWorldSave* CurrentSave = Cast<UWorldSave>(UUsefulFunctions::LoadSave(TargetSaveClass, Index, false));

	if(!IsValid(CurrentSave)){
		CurrentSave = Cast<UWorldSave>(UUsefulFunctions::CreateSave(TargetSaveClass, Index));
	}

	else if(!CurrentSave->IsA(TargetSaveClass)){
		CurrentSave = Cast<UWorldSave>(UUsefulFunctions::CreateSave(TargetSaveClass, Index));
	}

	CurrentSave->WorldName = GetWorld()->GetName();

	//Player
	CurrentSave->PlayerTransform = MainPlayer->GetActorTransform();
	CurrentSave->PlayerCameraRotation = MainPlayer->GetController()->GetControlRotation();
	CurrentSave->PlayerGolds = MainPlayer->GetGolds();

	CurrentSave->TchatLinesList = MainPlayerController->GetTchatWidget()->GetAllTchatLines();

	if(MainPlayer->GetMark()->GetIsMarkPlaced()){
		CurrentSave->MarkTransform = MainPlayer->GetMark()->GetActorTransform();
		CurrentSave->bIsMarkPlaced = true;
	} else{
		CurrentSave->bIsMarkPlaced = false;
	}

	//Doors
	TArray<AActor*> DoorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasicDoor::StaticClass(), DoorList);

	CurrentSave->DoorDataList.Empty();
	for(int i = 0; i < DoorList.Num(); i++){
		const ABasicDoor* CurrentDoor = Cast<ABasicDoor>(DoorList[i]);

		CurrentSave->DoorDataList.Add(CurrentDoor->GetName(), CurrentDoor->SaveDoor());
	}

	//Tchat Box
	TArray<AActor*> TchaBoxList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATchatTriggerBox::StaticClass(), TchaBoxList);

	CurrentSave->TchatTriggerNameList.Empty();
	for(int i = 0; i < TchaBoxList.Num(); i++){
		const ATchatTriggerBox* CurrentTchatBox = Cast<ATchatTriggerBox>(TchaBoxList[i]);

		CurrentSave->TchatTriggerNameList.Add(CurrentTchatBox->GetName());
	}

	//Ban Logs
	TArray<AActor*> BanLogList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABanLog::StaticClass(), BanLogList);

	CurrentSave->BanLogList.Empty();
	for(int i = 0; i < BanLogList.Num(); i++){
		const ABanLog* CurrentBanLog = Cast<ABanLog>(BanLogList[i]);

		CurrentSave->BanLogList.Add(CurrentBanLog->GetName(), CurrentBanLog->IsActivated());
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

	UUsefulFunctions::SaveToSlot(CurrentSave, Index);
}

void AGlitchUEGameMode::GlobalWorldLoad(const int Index){
	ISaveInterface::GlobalWorldLoad(Index);

	const UWorldSave* CurrentSave = Cast<UWorldSave>(UUsefulFunctions::LoadSave(UWorldSave::StaticClass(), Index, false));

	if(CurrentSave == nullptr){
		return;
	}

	FString LoadOptions = "WorldSaveLoad|";

	LoadOptions += FString::FromInt(Index);

	UGameplayStatics::OpenLevel(this, FName(*CurrentSave->WorldName), true, LoadOptions);
}

void AGlitchUEGameMode::LaunchStealthTimer(float TimerValue){
	if(MainPlayerController->GetTimerWidget()->IsTimerRunning()){
		return;
	}

	if(TimerValue <= 0){
		TimerValue = StealthTimer;
	}

	OnStartStealthTimer.Broadcast();

	FKOnFinishTimer EndEvent;
	EndEvent.BindDynamic(this, &AGlitchUEGameMode::EndStealthTimer);

	MainPlayerController->GetTimerWidget()->StartTimer(TimerValue, EndEvent, false);
}

float AGlitchUEGameMode::GetStealthTimer() const{
	return StealthTimer;
}

float AGlitchUEGameMode::GetGoldTimerMultiplier() const{
	return GoldTimerMultiplier;
}

bool AGlitchUEGameMode::CanStartTowerDefense() const{
	return CurrentActivatedCatalyseurs >= MaxCatalyseurToActivate;
}

void AGlitchUEGameMode::ForceEndStealthPhase() const{
	Nexus->GetActivableComp()->ActivateObject();
}

void AGlitchUEGameMode::UpdateActivatedCatalyseurAmount(const bool Increase){
	Increase ? CurrentActivatedCatalyseurs++ : CurrentActivatedCatalyseurs--;

	if(CurrentPhase == EPhases::TowerDefense){
		return;
	}

	Nexus->SetCanInteractWithNexus(CanStartTowerDefense());
	UpdatePlayerObjectives();
}

int AGlitchUEGameMode::GetActivatedCatalyseurNum() const{
	return CurrentActivatedCatalyseurs;
}

bool AGlitchUEGameMode::UseAutoObjectivesForPlayer() const{
	return bUseAutoObjectivesForPlayer;
}

UWorldSave* AGlitchUEGameMode::StealthWorldSave(UWorldSave* CurrentSave){
	UStealthSave* CastedSave = Cast<UStealthSave>(CurrentSave);

	// Inhibiteurs
	TArray<AActor*> InhibiteurList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInhibiteur::StaticClass(), InhibiteurList);

	CastedSave->InhibiteurStateList.Empty();
	for(int i = 0; i < InhibiteurList.Num(); i++){
		AInhibiteur* CurrentInhibiteur = Cast<AInhibiteur>(InhibiteurList[i]);

		CastedSave->InhibiteurStateList.Add(CurrentInhibiteur->GetName(), CurrentInhibiteur->GetActivableComp()->IsActivated());
	}

	TArray<AActor*> AIList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainAIController::StaticClass(), AIList);

	CastedSave->AIDataList.Empty();
	for(int i = 0; i < AIList.Num(); i++){
		AMainAIController* CurrentController = Cast<AMainAIController>(AIList[i]);

		CastedSave->AIDataList.Add(CurrentController->GetControllerName(), CurrentController->SaveAI());
	}

	CastedSave->LevelState = LevelState;

	CastedSave->bIsStealthTimeRunning = MainPlayerController->GetTimerWidget()->IsTimerRunning();

	if(CastedSave->bIsStealthTimeRunning){
		CastedSave->RemainingStealthTime = MainPlayerController->GetTimerWidget()->GetTimerElapsed();
	}

	return CastedSave;
}

UWorldSave* AGlitchUEGameMode::TowerDefenseWorldSave(UWorldSave* CurrentSave){
	UTowerDefenseSave* CastedSave = Cast<UTowerDefenseSave>(CurrentSave);

	CastedSave->CurrentActivatedCatalyseurNumber = CurrentActivatedCatalyseurs;

	CastedSave->PlayerCrystalName = IsValid(MainPlayer->GetCurrentDrone()) ? MainPlayer->GetCurrentDrone()->GetName() : "";

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

	TArray<AActor*> CatalyseurList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACatalyseur::StaticClass(), CatalyseurList);

	CastedSave->CatalyseurDataList.Empty();
	for(int i = 0; i < CatalyseurList.Num(); i++){

		ACatalyseur* CurrentCatalyseur = Cast<ACatalyseur>(CatalyseurList[i]);

		CastedSave->CatalyseurDataList.Add(CurrentCatalyseur->GetName(), CurrentCatalyseur->SaveCatalyseur());
	}

	TArray<AActor*> CrystalList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APursuitDrone::StaticClass(), CrystalList);

	CastedSave->CrystalTransformList.Empty();
	for(int i = 0; i < CrystalList.Num(); i++){

		const APursuitDrone* CurrentCrystal = Cast<APursuitDrone>(CrystalList[i]);

		// if the crystal isn't attached to a placable or the player
		if(!IsValid(CurrentCrystal->GetAttachParentActor())){
			CastedSave->CrystalTransformList.Add(CurrentCrystal->GetName(), CurrentCrystal->GetActorTransform());
		}
	}

	CastedSave->NexusHealth = Nexus->GetHealthComp()->GetCurrentHealth();

	CastedSave->CurrentWave = WaveManager->GetCurrentWaveNumber();

	return CastedSave;
}

UWorldSave* AGlitchUEGameMode::StealthWorldLoad(UWorldSave* CurrentSave){
	UStealthSave* CastedSave = Cast<UStealthSave>(CurrentSave);

	//Inhibiteur
	TArray<AActor*> InhibiteurList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInhibiteur::StaticClass(), InhibiteurList);

	TArray<FString> FStringArray;
	CastedSave->InhibiteurStateList.GetKeys(FStringArray);

	for(int i = 0; i < InhibiteurList.Num(); i++){
		for(int j = 0; j < FStringArray.Num(); j++){
			if(InhibiteurList[i]->GetName() == FStringArray[j]){

				AInhibiteur* CurrentInhibiteur = Cast<AInhibiteur>(InhibiteurList[i]);

				if(CastedSave->InhibiteurStateList.FindRef(InhibiteurList[i]->GetName())){
					CurrentInhibiteur->GetActivableComp()->ActivateObject();
				}

			}
		}
	}

	TArray<AActor*> AIList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainAIController::StaticClass(), AIList);

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

	if(CastedSave->bIsStealthTimeRunning){
		LaunchStealthTimer(CastedSave->RemainingStealthTime);
	}

	SetLevelState(CastedSave->LevelState);

	return CastedSave;
}

UWorldSave* AGlitchUEGameMode::TowerDefenseWorldLoad(UWorldSave* CurrentSave){
	UTowerDefenseSave* CastedSave = Cast<UTowerDefenseSave>(CurrentSave);

	Nexus->GetHealthComp()->SetHealth(CastedSave->NexusHealth);

	CurrentActivatedCatalyseurs = CastedSave->CurrentActivatedCatalyseurNumber;

	ForceEndStealthPhase();

	// this skip the preparation time
	WaveManager->SetWave(CastedSave->CurrentWave);

	TArray<AActor*> PursuitDroneList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APursuitDrone::StaticClass(), PursuitDroneList);

	for(int i = 0; i < CastedSave->PlacableDataList.Num(); i++){
		const TSubclassOf<APlacableActor> TargetClass = CastedSave->PlacableDataList[i].PlacableActorClass;
		const FVector TargetLocation = CastedSave->PlacableDataList[i].ActorTransform.GetLocation();
		const FRotator TargetRotation = CastedSave->PlacableDataList[i].ActorTransform.Rotator();

		APlacableActor* CurrentPlacableActor = GetWorld()->SpawnActor<APlacableActor>(TargetClass, TargetLocation, TargetRotation, FActorSpawnParameters());
		CurrentPlacableActor->InitializePlacable(CastedSave->PlacableDataList[i], PursuitDroneList);
		CurrentPlacableActor->SetMissingData(Nexus, MainPlayer);
	}

	TArray<FString> FStringArray;
	CastedSave->CrystalTransformList.GetKeys(FStringArray);

	for(int i = 0; i < PursuitDroneList.Num(); i++){
		if(CastedSave->PlayerCrystalName == PursuitDroneList[i]->GetName()){
			APursuitDrone* PlayerDrone = Cast<APursuitDrone>(PursuitDroneList[i]);
			MainPlayer->SetCurrentDrone(PlayerDrone);
			PlayerDrone->EnableSpinBehavior();
			continue;
		}

		for(int j = 0; j < FStringArray.Num(); j++){
			if(PursuitDroneList[i]->GetName() == FStringArray[j]){
				PursuitDroneList[i]->SetActorTransform(CastedSave->CrystalTransformList.FindRef(FStringArray[j]));
				break;
			}
		}
	}

	TArray<AActor*> CatalyseurList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACatalyseur::StaticClass(), CatalyseurList);

	const float CatalyseurCompletionPercent = 100.0f/CatalyseurList.Num() * GetActivatedCatalyseurNum();

	Dissolver->ForceDissolverValue(CatalyseurCompletionPercent * Dissolver->GetMaxRadius() / 100.0f);

	FStringArray.Empty();
	CastedSave->CatalyseurDataList.GetKeys(FStringArray);

	for(int i = 0; i < CatalyseurList.Num(); i++){
		for(int j = 0; j < FStringArray.Num(); j++){
			if(CatalyseurList[i]->GetName() == FStringArray[j]){

				ACatalyseur* CurrentCatalyseur = Cast<ACatalyseur>(CatalyseurList[i]);

				CurrentCatalyseur->LoadCatalyseur(CastedSave->CatalyseurDataList.FindRef(CatalyseurList[i]->GetName()));
			}
		}
	}

	return CastedSave;
}

void AGlitchUEGameMode::EndStealthTimer(){
	CanStartTowerDefense() ? ForceEndStealthPhase() : MainPlayer->KillPlayer();
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

		if(OptionsString == ""){
			WaveManager->StartPrepareTimer();
		}

		#if !UE_BUILD_SHIPPING
			// for launch game
			else if(OptionsString == "?Name=Player"){
				WaveManager->StartPrepareTimer();
			}
		#endif

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

void AGlitchUEGameMode::RemoveStealthTime(const float RemoveTime) const{
	if(!MainPlayerController->GetTimerWidget()->IsTimerRunning()){
		return;
	}

	const float CurrentTime = MainPlayerController->GetTimerWidget()->GetTimerElapsed();

	const float NewTime = FMath::Clamp(CurrentTime - RemoveTime, 1.0f, StealthTimer);

	MainPlayerController->GetTimerWidget()->ChangeTimerValue(NewTime);
}

void AGlitchUEGameMode::AddGlitch(const float AddedValue){
	GlitchValue = FMath::Clamp(AddedValue + GlitchValue, 0.0f, GlitchMaxValue);

	MainPlayer->UpdateGlitchGaugeFeedback(GlitchValue, GlitchMaxValue);

	if (GlitchValue == GlitchMaxValue) {

		Glitch::EGlitchEvents RandomGlitchType = Glitch::EGlitchEvents::UpgradePlayer;

		switch (CurrentPhase){
			case EPhases::Infiltration:
				RandomGlitchType = static_cast<Glitch::EGlitchEvents>(FMath::RandRange(MainPlayerController->GetTimerWidget()->IsTimerRunning() ? 0 : Glitch::StealthIndex, Glitch::BothIndex));
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

void AGlitchUEGameMode::CallKeyboardEnding(){
	MainPlayerController->UnbindPause();
	MainPlayerController->GetTchatWidget()->AddMultipleTchatLines(KeyboardEndingMessages);

	MainPlayerController->GetTchatWidget()->OnFinishWritingMessageList.AddDynamic(this, &AGlitchUEGameMode::KeyboardMessagesEnd);
}

void AGlitchUEGameMode::KeyboardMessagesEnd(){
	MainPlayerController->GetTchatWidget()->OnFinishWritingMessageList.RemoveDynamic(this, &AGlitchUEGameMode::KeyboardMessagesEnd);

	const FOnTimelineEvent EmptyEvent;
	Cast<AAudioManager>(UGameplayStatics::GetActorOfClass(MainPlayer, AAudioManager::StaticClass()))->FadeOutMusic(EmptyEvent, TransitionDuration);

	GetWorld()->GetTimerManager().SetTimer(GlitchEffectEndingTimerHandle, [&](){
		Cast<UCreditsScreen>(CreateWidget(MainPlayerController, CreditsScreenWidgetClass))->SetEndMessage(EndKeyboardMessage);
	}, TransitionDuration, false);
}

void AGlitchUEGameMode::CallNexusEnding(){
	MainPlayerController->UnbindPause();

	Dissolver->DissolveToAtSpeed(0, EndDissolveDuration);

	const FOnTimelineEvent EmptyEvent;
	Cast<AAudioManager>(UGameplayStatics::GetActorOfClass(MainPlayer, AAudioManager::StaticClass()))->FadeOutMusic(EmptyEvent, EndDissolveDuration);

	GetWorld()->GetTimerManager().SetTimer(DissolveEndingTimerHandle, [&](){
		MainPlayer->EnableGlitchEffect(true, EndDissolveDuration, 9999);

		GetWorld()->GetTimerManager().SetTimer(GlitchEffectEndingTimerHandle, [&](){
			MainPlayer->EnableGlitchEffect(false, TransitionDuration, 0);
			Cast<UCreditsScreen>(CreateWidget(MainPlayerController, CreditsScreenWidgetClass))->SetEndMessage(EndNexusMessage);
		}, TransitionDuration, false);
	}, EndDissolveDuration, false);
}

void AGlitchUEGameMode::GlitchUpgradeAlliesUnits() const{
	TArray<AActor*> PlacableActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlacableActor::StaticClass(), PlacableActorList);

	PlacableActorList = UUsefulFunctions::SortActorsByDistanceToActor(PlacableActorList, MainPlayer);

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
		AIList = UUsefulFunctions::SortActorsByDistanceToActor(AIList, MainPlayer);

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
	RemoveStealthTime(GlitchReduceStealthTimer);
}

#pragma region ConsoleCommands

void AGlitchUEGameMode::SetSelfTimeDilation(float TimeDilation) const{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn()->CustomTimeDilation = TimeDilation;
}

void AGlitchUEGameMode::NextWave() const{
	WaveManager->ForceNextWave();
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
		MainPlayerController->BindNormalMode();

		ActorList[0]->Destroy();
	}
}

void AGlitchUEGameMode::Dissolve(const float Value) const{
	Dissolver->DissolveTo(Value);
}

void AGlitchUEGameMode::PauseStealthTimer(const bool bPause) const{
	if(!MainPlayerController->GetTimerWidget()->IsTimerRunning()){
		return;
	}

	MainPlayerController->GetTimerWidget()->PauseTimer(bPause);
}

#pragma endregion
