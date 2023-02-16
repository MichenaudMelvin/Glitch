// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SightComponent.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/MainPlayer.h"

USightComponent::USightComponent(){
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/Meshs/CustomCollisions/SM_SightCollision"));

	SetStaticMesh(Mesh.Object);
	bCanEverAffectNavigation = false;
}

void USightComponent::BeginPlay(){
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &USightComponent::EnterSight);
	OnComponentEndOverlap.AddDynamic(this, &USightComponent::ExitSight);

	OwnerBlackboard = Cast<APawn>(GetOwner())->Controller->FindComponentByClass<UBlackboardComponent>();

	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetCanEverAffectNavigation(false);
}

void USightComponent::Check(){
	// if player in sight
	OwnerBlackboard->SetValueAsVector("InvestigationLocation", SightActor->GetActorLocation());

	if(bPlayerInCollision){
		if(IsValid(SightActor) && UUsefullFunctions::CanSee(GetOwner(), GetComponentLocation(), SightActor, ECC_EngineTraceChannel3) && bPlayerInCollision){
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
		bPlayerInCollision = true;
		GetWorld()->GetTimerManager().ClearTimer(SightTimer);

		GetWorld()->GetTimerManager().SetTimer(SightTimer, [&]() {
			Check();
		}, 0.1f, true);
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

bool USightComponent::IsSomethingInSight(){
	return IsValid(SightActor);
}

bool USightComponent::IsPlayerInSight(){
	return bIsPlayerInSight;
}
