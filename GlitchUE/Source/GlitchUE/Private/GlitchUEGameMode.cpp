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
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "GameFramework/AsyncActionHandleSaveGame.h"
#include "Saves/AbstractSave.h"
#include "Kismet/KismetMathLibrary.h"

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

UAbstractSave* AGlitchUEGameMode::Load(TSubclassOf<UAbstractSave> SaveClass, int UserIndex){
	return Cast<UAbstractSave>(UGameplayStatics::LoadGameFromSlot(SaveClass.GetDefaultObject()->GetSlotName(), UserIndex));
}

void AGlitchUEGameMode::FastSave_Implementation(){}

void AGlitchUEGameMode::FastLoad_Implementation(){}

EPhases AGlitchUEGameMode::GetPhases(){
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

ELevelState AGlitchUEGameMode::GetLevelState(){
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

void AGlitchUEGameMode::UpdateLevelColor(FLinearColor NewColor) {
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

		//OnGlitchMax.Broadcast();
		CheckAvailableGlitchEvents();
		EGlitchEvent::Type RandomGlitchType;

		while (true){
			RandomGlitchType = static_cast<EGlitchEvent::Type>(FMath::RandRange(0, 3));
		}

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

void AGlitchUEGameMode::GlitchUpgradeAlliesUnits(){
	UE_LOG(LogTemp, Warning, TEXT("UpgradeAlliesUnits"));

	TArray<AActor*> PlacableActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlacableActor::StaticClass(), PlacableActorList);

	if (PlacableActorList.Num() > 0) {
		PlacableActorList = UUsefullFunctions::SortActorsByDistanceToActor(PlacableActorList, MainPlayer);
		Cast<APlacableActor>(PlacableActorList[0])->GlitchUpgrade();
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Aucune tourlles dans le monde"));
	}
}

void AGlitchUEGameMode::GlitchUpgradeEnemiesAI(){
	UE_LOG(LogTemp, Warning, TEXT("UpgradeEnemiesAI"));
	
	TArray<AActor*> AIList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainAICharacter::StaticClass(), AIList);

	if (AIList.Num() > 0) {
		AIList = UUsefullFunctions::SortActorsByDistanceToActor(AIList, MainPlayer);
		Cast<AMainAICharacter>(AIList[0])->GlitchUpgrade();
	} else{
		UE_LOG(LogTemp, Warning, TEXT("Aucun ennemis dans le monde"));
	}

}

void AGlitchUEGameMode::GlitchUpgradePlayer() {
	UE_LOG(LogTemp, Warning, TEXT("UpgradePlayer"));

	MainPlayer->GlitchUpgrade();
}

void AGlitchUEGameMode::GlitchRandomFX() {
	UE_LOG(LogTemp, Warning, TEXT("RandomFX"));
}

void AGlitchUEGameMode::CheckAvailableGlitchEvents(){
	TArray<AActor*> PlacableActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlacableActor::StaticClass(), PlacableActorList);

	TArray<AActor*> AIList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainAICharacter::StaticClass(), AIList);

}

#pragma region ConsoleCommands

void AGlitchUEGameMode::SetGlobalTimeDilation(float TimeDilation){
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
}

void AGlitchUEGameMode::NextWave(){
	WaveManager->SetWave(WaveManager->GetCurrentWaveNumber() + 1);
}

void AGlitchUEGameMode::GoToWave(int NewWave){
	WaveManager->SetWave(NewWave);
}

void AGlitchUEGameMode::CrashGame(){
	UE_LOG(LogTemp, Fatal, TEXT("CRASH"));
}

#pragma endregion