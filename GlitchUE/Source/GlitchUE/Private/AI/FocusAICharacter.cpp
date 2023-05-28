// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FocusAICharacter.h"

AFocusAICharacter::AFocusAICharacter(){
	HealthWidget = CreateDefaultSubobject<UBillboardWidgetComponent>(TEXT("Health Bar"));
	HealthWidget->SetupAttachment(GetMesh());

	HealthWidget->SetRelativeLocation(FVector(0, 0, 100));
	HealthWidget->SetRelativeRotation(FRotator(0, 180, 0));
	HealthWidget->SetDrawSize(FVector2D(100, 30));

	HealthComp->OnHealthChange.AddDynamic(this, &AFocusAICharacter::UpdateWidgetHealth);
}

void AFocusAICharacter::BeginPlay(){
	Super::BeginPlay();

	EnemyHealthBarWidget = Cast<UEnemyHealthBar>(HealthWidget->GetWidget());
	UpdateWidgetHealth();
}

void AFocusAICharacter::UpdateWidgetHealth(){
	if(IsValid(EnemyHealthBarWidget)){
		EnemyHealthBarWidget->UpdateHealth(HealthComp->GetNormalizedHealth());
	}
}
