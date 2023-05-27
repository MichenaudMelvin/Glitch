// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SightComponent.h"
#include "PopcornFXFunctions.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/MainPlayer.h"

USightComponent::USightComponent(){
	UPrimitiveComponent::SetCollisionResponseToAllChannels(ECR_Overlap);
	bCanEverAffectNavigation = false;

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> SightEffectObject(TEXT("/Game/VFX/Particles/FX_Enemies/Pk_Surveillance"));
	check(SightEffectObject.Succeeded());

	SightEffect = SightEffectObject.Object;
}

void USightComponent::BeginPlay(){
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &USightComponent::EnterSight);
	OnComponentEndOverlap.AddDynamic(this, &USightComponent::ExitSight);

	OwnerBlackboard = Cast<APawn>(GetOwner())->Controller->FindComponentByClass<UBlackboardComponent>();

	if(bDrawFX){
		SightFX = UPopcornFXFunctions::SpawnEmitterAttached(SightEffect, this, "PopcornFX_DefaultScene", "None", FVector::ZeroVector, FRotator(0, 90, 0),EAttachLocation::SnapToTarget, true, false);
	}
}

void USightComponent::OnComponentDestroyed(bool bDestroyingHierarchy){
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	if(IsValid(SightFX)){
		SightFX->DestroyComponent();
	}
}

void USightComponent::Check(){
	// if player in sight
	OwnerBlackboard->SetValueAsVector("InvestigationLocation", SightActor->GetActorLocation());

	if(bPlayerInCollision && !Cast<AMainPlayer>(SightActor)->IsInGlitchZone()){
		if(UUsefullFunctions::CanSee(GetOwner(), GetComponentLocation(), SightActor, ECC_EngineTraceChannel3)){
			InvestigateTimeElapsed += 0.1f;

			OwnerBlackboard->SetValueAsBool("Investigate", true);
			OnSightPlayer.Broadcast((InvestigateTimeElapsed * 100) / MaxInvestigateTime);

			if(InvestigateTimeElapsed >= MaxInvestigateTime){
				bIsPlayerInSight = true;

				OwnerBlackboard->SetValueAsObject("Player", SightActor);
				OwnerBlackboard->SetValueAsVector("PlayerLocation", SightActor->GetActorLocation());
				OwnerBlackboard->SetValueAsBool("Investigate", false);

				GetWorld()->GetTimerManager().ClearTimer(SightTimer);
			}
		}
	} else{
		InvestigateTimeElapsed -= 0.1f;
		OnLooseSightPlayer.Broadcast((InvestigateTimeElapsed * 100) / MaxInvestigateTime);

		if(InvestigateTimeElapsed <= 0){

			GetWorld()->GetTimerManager().ClearTimer(SightTimer);
			InvestigateTimeElapsed = 0;

			LooseSight();
		}
	}
}

void USightComponent::LooseSight(){
	OwnerBlackboard->SetValueAsBool("Investigate", false);

	GetWorld()->GetTimerManager().ClearTimer(LooseSightTimer);
	bIsPlayerInSight = false;
	SightActor = nullptr;
}

void USightComponent::EnterSight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		SightActor = OtherActor;

		if(!Cast<AMainPlayer>(SightActor)->IsInGlitchZone()){
			bPlayerInCollision = true;
			GetWorld()->GetTimerManager().ClearTimer(SightTimer);

			GetWorld()->GetTimerManager().SetTimer(SightTimer, this, &USightComponent::Check, 0.1f, true);
		}
	}
}

void USightComponent::ExitSight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		bPlayerInCollision = false;

		if(bIsPlayerInSight){
			LooseSight();
		}
	}
}

bool USightComponent::IsSomethingInSight() const{
	if(!IsValid(SightActor)){
		return false;
	}

	if(Cast<AMainPlayer>(SightActor)->IsInGlitchZone()){
		return false;
	}

	return true;
}

bool USightComponent::IsPlayerInSight() const{
	return bIsPlayerInSight;
}

float USightComponent::GetTimerElapsed() const{
	return InvestigateTimeElapsed;
}
