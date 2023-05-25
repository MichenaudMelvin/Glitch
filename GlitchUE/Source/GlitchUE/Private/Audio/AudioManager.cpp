// Fill out your copyright notice in the Description page of Project Settings.

#include "Audio/AudioManager.h"
#include "Kismet/GameplayStatics.h"

AAudioManager::AAudioManager(){
	PrimaryActorTick.bCanEverTick = true;

	FMODAudioComp = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("FMOD Audio"));
	SetRootComponent(FMODAudioComp);

	FMODAudioComp->SetMobility(EComponentMobility::Static);

	static ConstructorHelpers::FObjectFinder<UFMODEvent> StMusic(TEXT("/Game/FMOD/Events/MUSIC/MUSIC_Infiltration"));
	check(StMusic.Succeeded());

	StealthMusic = StMusic.Object;
	
	FMODAudioComp->Event = StealthMusic;

	static ConstructorHelpers::FObjectFinder<UFMODEvent> StAlarm(TEXT("/Game/FMOD/Events/SFX/SFX_detected_alarm"));
	check(StAlarm.Succeeded());

	StealthAlarm = StAlarm.Object;

	static ConstructorHelpers::FObjectFinder<UFMODEvent> TWDMusic(TEXT("/Game/FMOD/Events/MUSIC/MUSIC_TowerDefense"));
	check(TWDMusic.Succeeded());

	TowerDefenseMusic = TWDMusic.Object;

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
}

void AAudioManager::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);

	VolumeTimeline.TickTimeline(DeltaSeconds);
	ParameterTimeline.TickTimeline(DeltaSeconds);
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
}

void AAudioManager::SetStealthAudio(const ELevelState LevelState){

	switch (LevelState){
		case ELevelState::Normal:
			FadeParameter("Stealth", false);
			break;
		case ELevelState::Alerted:
			//FMODAudioComp->SetEvent(StealthAlarm);
			//FMODAudioComp->Play();
			//FMODAudioComp->OnEventStopped.AddDynamic(this, &AAudioManager::SetStealthMusic);
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

void AAudioManager::UpdateTowerDefenseMusic() {
	TowerDefenseLayer++;

	if (TowerDefenseLayer == 4){
		TowerDefenseLayer = 0;
		FMODAudioComp->Play();
	}

	FMODAudioComp->SetParameter("TowerDefense", TowerDefenseLayer);
}

UFMODAudioComponent* AAudioManager::GetAudioComp() const{
	return FMODAudioComp;
}

