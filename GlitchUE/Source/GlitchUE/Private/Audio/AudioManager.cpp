// Fill out your copyright notice in the Description page of Project Settings.

#include "Audio/AudioManager.h"

#include "FMODBlueprintStatics.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayer.h"

AAudioManager::AAudioManager(){
	PrimaryActorTick.bCanEverTick = true;

	FMODAudioComp = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("FMOD Audio"));
	SetRootComponent(FMODAudioComp);

	FMODAudioComp->SetMobility(EComponentMobility::Static);

	static ConstructorHelpers::FObjectFinder<UFMODEvent> StMusic(TEXT("/Game/FMOD/Events/MUSIC/MUSIC_Infiltration"));
	check(StMusic.Succeeded());

	StealthMusic = StMusic.Object;

	StartMusic = StealthMusic;

	FMODAudioComp->Event = StartMusic;

	static ConstructorHelpers::FObjectFinder<UFMODEvent> StAlarm(TEXT("/Game/FMOD/Events/SFX/SFX_detected_alarm"));
	check(StAlarm.Succeeded());

	StealthAlarm = StAlarm.Object;

	static ConstructorHelpers::FObjectFinder<UFMODEvent> TWDMusic(TEXT("/Game/FMOD/Events/MUSIC/MUSIC_TowerDefense"));
	check(TWDMusic.Succeeded());
	TowerDefenseMusic = TWDMusic.Object;

	static ConstructorHelpers::FObjectFinder<UFMODEvent> PMusic(TEXT("/Game/FMOD/Events/MUSIC/MUSIC_Pause"));
	check(PMusic.Succeeded());

	PauseMusic = PMusic.Object;

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_ZeroToOneCurve"));
	check(Curve.Succeeded());

	ZeroToOneCurve = Curve.Object;
}

void AAudioManager::BeginPlay(){
	Super::BeginPlay();

	if(UGameplayStatics::GetGameMode(GetWorld())->IsA(AGlitchUEGameMode::StaticClass())){
		GameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		GameMode->OnSwitchLevelState.AddDynamic(this, &AAudioManager::SetStealthAudio);
	}

	FOnTimelineFloat UpdateEvent;

	UpdateEvent.BindDynamic(this, &AAudioManager::FadeVolume);

	VolumeTimeline.AddInterpFloat(ZeroToOneCurve, UpdateEvent);

	UpdateEvent.Clear();
	UpdateEvent.BindDynamic(this, &AAudioManager::FadeParameterValue);

	ParameterTimeline.AddInterpFloat(ZeroToOneCurve, UpdateEvent);

	Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AAudioManager::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);

	VolumeTimeline.TickTimeline(DeltaSeconds);
	ParameterTimeline.TickTimeline(DeltaSeconds);
}

void AAudioManager::OnConstruction(const FTransform& Transform){
	Super::OnConstruction(Transform);

	FMODAudioComp->SetEvent(StartMusic);
}

void AAudioManager::FadeVolume(float Alpha){
	FMODAudioComp->SetVolume(Alpha);
}

void AAudioManager::FadeParameterValue(float Alpha){
	FMODAudioComp->SetParameter(CurrentParameter, Alpha);
}

void AAudioManager::SetStealthMusic(){
	FMODAudioComp->SetEvent(StealthMusic);
	FMODAudioComp->Play();
}

void AAudioManager::SetTowerDefenseMusic() {
	FMODAudioComp->SetEvent(TowerDefenseMusic);

	const FOnTimelineEvent EmptyEvent;
	FadeInMusic(EmptyEvent, FadePhaseTransition);
	UpdateTowerDefenseMusic();
}

void AAudioManager::SetPauseMusic() {
	FMODAudioComp->SetEvent(PauseMusic);
	FMODAudioComp->Play();
}

void AAudioManager::SetStealthAudio(const ELevelState LevelState){

	switch (LevelState) {
	case ELevelState::Normal:
		FadeParameter("Stealth", false);
		break;
	case ELevelState::Alerted:
		FadeParameter("Stealth");
		break;
	}
}

void AAudioManager::FadeParameter(const FName Param, const bool bFadeIn){
	CurrentParameter = Param;

	bFadeIn ? ParameterTimeline.Play() : ParameterTimeline.Reverse();
}

void AAudioManager::SetParameter(const FName Param, const float Value) const{
	FMODAudioComp->SetParameter(Param, Value);
}

void AAudioManager::FadeInMusic(const FOnTimelineEvent FinishEvent, const float FadeDuration){
	VolumeTimeline.SetTimelineFinishedFunc(FinishEvent);

	VolumeTimeline.SetPlayRate(1/FadeDuration);

	VolumeTimeline.PlayFromStart();
}

void AAudioManager::FadeOutMusic(const FOnTimelineEvent FinishEvent, const float FadeDuration){
	VolumeTimeline.SetTimelineFinishedFunc(FinishEvent);

	VolumeTimeline.SetPlayRate(1/FadeDuration);

	VolumeTimeline.ReverseFromEnd();
}

void AAudioManager::SwitchToTowerDefenseMusic(){
	FOnTimelineEvent SwitchMusicEvent;

	SwitchMusicEvent.BindDynamic(this, &AAudioManager::SetTowerDefenseMusic);

	FadeOutMusic(SwitchMusicEvent, FadePhaseTransition);
}

void AAudioManager::SwitchToPauseMusic() {
	FOnTimelineEvent SwitchMusicEvent;

	SwitchMusicEvent.BindDynamic(this, &AAudioManager::SetPauseMusic);

	FadeOutMusic(SwitchMusicEvent, FadePhaseTransition);
}

void AAudioManager::UpdateTowerDefenseMusic(){
	// Tableau pour stocker les valeurs possibles
	TArray<int> valeurs = { 0, 1, 2 };

	// Mélange des valeurs dans le tableau
	for (int i = 0; i < valeurs.Num(); i++) {
		int j = valeurs[FMath::FRandRange(0, valeurs.Num() - 1)];
		valeurs.Swap(valeurs[i], valeurs[j]);
	}

	// Attribution des valeurs aux variables
	FMODAudioComp->SetParameter("Accom", valeurs[0]);
	FMODAudioComp->SetParameter("Melod", valeurs[1]);
	FMODAudioComp->SetParameter("Perc", valeurs[2]);

	//FMODAudioComp->SetParameter("TowerDefense", TowerDefenseLayer);
}

UFMODAudioComponent* AAudioManager::GetAudioComp() const{
	return FMODAudioComp;
}
