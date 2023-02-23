// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICamera/AICameraPawn.h"
#include "AI/MainAIController.h"
#include "AI/UI/SightIndication.h"

AAICameraPawn::AAICameraPawn(){
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMeshComp);

	SightComp = CreateDefaultSubobject<USightComponent>(TEXT("CameraDirection"));
	SightComp->SetupAttachment(StaticMeshComp);

	SightWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("SightIndication"));
	SightWidget->SetupAttachment(StaticMeshComp);

	SightWidget->SetRelativeLocation(FVector(0, 0, 200));

	SightWidget->SetWorldScale3D(FVector(0.25,0.05,0.05));
	SightWidget->SetDrawSize(FVector2D(1920, 1080));
}

void AAICameraPawn::BeginPlay(){
	Super::BeginPlay();

	Blackboard = Cast<AMainAIController>(GetController())->GetBlackboardComponent();
	
	Blackboard->SetValueAsVector("OriginalPosition", GetActorLocation());
	Blackboard->SetValueAsEnum("CameraBehavior", static_cast<uint8>(CameraBehavior));

	switch (CameraBehavior){
	case ECameraBehavior::Static:
		break;
	
	case ECameraBehavior::Clamped: 
		Blackboard->SetValueAsFloat("MaxRotation", MaxRotation);
		Blackboard->SetValueAsFloat("MinRotation", MinRotation);
		Blackboard->SetValueAsFloat("SpeedRotation", SpeedRoation);
		break;
	
	case ECameraBehavior::FullRotation: 
		Blackboard->SetValueAsFloat("SpeedRotation", SpeedRoation);
		break;
	}
	
	USightIndication* Widget = Cast<USightIndication>(SightWidget->GetWidget());
	SightComp->OnSightPlayer.AddDynamic(Widget, &USightIndication::UpdateSightIndication);
	SightComp->OnLooseSightPlayer.AddDynamic(Widget, &USightIndication::UpdateSightIndication);

}
