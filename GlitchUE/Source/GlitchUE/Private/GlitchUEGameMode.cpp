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
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Helpers/Debug/DebugPawn.h"
#include "Curves/CurveLinearColor.h"
#include "Saves/AbstractSave.h"

AGlitchUEGameMode::AGlitchUEGameMode(){
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/Player/BP_MainPlayer"));
	// if (PlayerPawnBPClass.Class != NULL)
	// {
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }

	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UCurveLinearColor> Color(TEXT("/Game/Blueprint/Curves/CC_LevelStateCurve"));
	check(Color.Succeeded());

	ColorCurve = Color.Object;
	
	static ConstructorHelpers::FObjectFinder<UCurveLinearColor> Blinking(TEXT("/Game/Blueprint/Curves/CC_BlinkingAlertCurve"));
	check(Blinking.Succeeded());

	BlinkingCurve = Blinking.Object;
	
	//static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> MPC(TEXT("/Game/VFX/Shaders/Dissolver/MPC_Dissolver"));
	//check(MPC.Succeeded());

	//AlertedMaterial = MPC.Object;
}

void AGlitchUEGameMode::BeginPlay() {
	Super::BeginPlay();

	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	TArray<AWaveManager*> WaveManagerArray;
	FindAllActors<AWaveManager>(GetWorld(), WaveManagerArray);
	if (WaveManagerArray.Num() == 0) {
		UE_LOG(LogTemp, Fatal, TEXT("AUCUN WAVE MANAGER N'EST PLACE DANS LA SCENE"));
	}

	WaveManager = WaveManagerArray[0];

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
}

void AGlitchUEGameMode::Tick(float deltaTime){
	Super::Tick(deltaTime);
	
	LevelStateTimeline.TickTimeline(deltaTime);
	BlinkingTimeline.TickTimeline(deltaTime);
}

void AGlitchUEGameMode::Save_Implementation(UAbstractSave* SaveObject){}

void AGlitchUEGameMode::FastSave_Implementation(){}

void AGlitchUEGameMode::FastLoad_Implementation(){}

EPhases AGlitchUEGameMode::GetPhases() const{
	return CurrentPhase;
}

void AGlitchUEGameMode::SetNewPhase(EPhases NewPhase){
	CurrentPhase = NewPhase;
	switch (CurrentPhase){
	case EPhases::Infiltration:
		break;
	case EPhases::TowerDefense:
		WaveManager->StartWave();
		break;
	}
}

ELevelState AGlitchUEGameMode::GetLevelState() const{
	return LevelState;
}

void AGlitchUEGameMode::SetLevelState(ELevelState NewState){
	LevelState = NewState;
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

void AGlitchUEGameMode::AddGlitch(float AddedValue){
	GlitchValue = FMath::Clamp(AddedValue + GlitchValue, 0.0f, GlitchMaxValue);
	if (GlitchValue == GlitchMaxValue) {

		switch (CurrentPhase){
			case EPhases::Infiltration:
				SetLevelState(ELevelState::Alerted);
				break;
			case EPhases::TowerDefense:
				break;
		}

		CheckAvailableGlitchEvents();
		EGlitchEvent::Type RandomGlitchType;

		RandomGlitchType = static_cast<EGlitchEvent::Type>(FMath::RandRange(0, 3));

		switch (RandomGlitchType){
		case EGlitchEvent::UpgradeAlliesUnits:
			GlitchUpgradeAlliesUnits();
			break;

		case EGlitchEvent::UpgradeEnemiesAI:
			GlitchUpgradeEnemiesAI();
			break;

		case EGlitchEvent::UpgradePlayer:
			GlitchUpgradePlayer();
			break;

		case EGlitchEvent::RandomFX:
			GlitchRandomFX();
			break;
		}

		GlitchValue = 0;
	}
}

float AGlitchUEGameMode::GetCurrentGlitchValue(){
	return GlitchValue;
}

void AGlitchUEGameMode::GlitchUpgradeAlliesUnits() const{
	UE_LOG(LogTemp, Warning, TEXT("UpgradeAlliesUnits"));

	TArray<AActor*> PlacableActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlacableActor::StaticClass(), PlacableActorList);
	UE_LOG(LogTemp, Warning, TEXT("The integer value is: %d"), PlacableActorList.Num());

	if (PlacableActorList.Num() > 0) {
		PlacableActorList = UUsefullFunctions::SortActorsByDistanceToActor(PlacableActorList, MainPlayer);

		for(int i = 0; i < PlacableActorList.Num(); i++){
			if(!PlacableActorList[i]->IsA(APreviewPlacableActor::StaticClass())){
				Cast<IGlitchInterface>(PlacableActorList[i])->ReciveGlitchUpgrade();
				return;
			}
		}
	}
}

void AGlitchUEGameMode::GlitchUpgradeEnemiesAI() const{
	UE_LOG(LogTemp, Warning, TEXT("UpgradeEnemiesAI"));
	
	TArray<AActor*> AIControllerList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainAIController::StaticClass(), AIControllerList);

	TArray<AActor*> AIList;
	for(int i = 0; i < AIControllerList.Num(); i++){
		AIList.Add(Cast<AMainAIController>(AIControllerList[i])->GetPawn());
	}

	if (AIList.Num() > 0) {
		AIList = UUsefullFunctions::SortActorsByDistanceToActor(AIList, MainPlayer);
		Cast<IGlitchInterface>(AIList[0])->ReciveGlitchUpgrade();
	} else{
		UE_LOG(LogTemp, Warning, TEXT("Aucun ennemis dans le monde"));
	}
}

void AGlitchUEGameMode::GlitchUpgradePlayer() const{
	UE_LOG(LogTemp, Warning, TEXT("UpgradePlayer"));

	Cast<IGlitchInterface>(MainPlayer)->ReciveGlitchUpgrade();
}

void AGlitchUEGameMode::GlitchRandomFX() const{
	UE_LOG(LogTemp, Warning, TEXT("RandomFX"));
}

void AGlitchUEGameMode::CheckAvailableGlitchEvents() const{
	TArray<AActor*> PlacableActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlacableActor::StaticClass(), PlacableActorList);

	TArray<AActor*> AIList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainAICharacter::StaticClass(), AIList);

}

#pragma region ConsoleCommands

void AGlitchUEGameMode::SetGlobalTimeDilation(float TimeDilation) const{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
}

void AGlitchUEGameMode::SetSelfTimeDilation(float TimeDilation) const{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn()->CustomTimeDilation = TimeDilation;
}

void AGlitchUEGameMode::NextWave() const{
	WaveManager->SetWave(WaveManager->GetCurrentWaveNumber() + 1);
}

void AGlitchUEGameMode::GoToWave(int NewWave) const{
	WaveManager->SetWave(NewWave);
}

void AGlitchUEGameMode::CrashGame() const{
	UE_LOG(LogTemp, Fatal, TEXT("CRASH"));
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
			MainPlayer->SetActorRotation(Cast<APawn>(ActorList[0])->Controller->GetControlRotation());
		}

		Cast<APawn>(ActorList[0])->Controller->Possess(MainPlayer);
		MainPlayer->GetMainPlayerController()->SelectNewGameplayMode(MainPlayer->GetMainPlayerController()->GetGameplayMode());

		ActorList[0]->Destroy();
	}
}

#pragma endregion
