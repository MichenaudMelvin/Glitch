// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableObject/Turret.h"
#include "Components/InteractableComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI/MainAICharacter.h"
#include "PlacableObject/TurretData.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"

ATurret::ATurret() {
	TurretBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBase"));
	SetRootComponent(TurretBase);

	TurretPillar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pillar"));
	TurretPillar->SetupAttachment(TurretBase);

	TurretHead = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	TurretHead->SetupAttachment(TurretPillar);
	TurretHead->SetRelativeLocation(FVector(0, 0, 100));

	TurretBase->CanCharacterStepUpOn = ECB_No;
	TurretPillar->CanCharacterStepUpOn = ECB_No;
	TurretHead->CanCharacterStepUpOn = ECB_No;

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_ZeroToOneCurve"));
	check(Curve.Succeeded());

	ZeroToOneCurve = Curve.Object;

	TurretVision = CreateDefaultSubobject<USphereComponent>(TEXT("TurretVision"));
	TurretVision->SetupAttachment(RootComponent);
	TurretVision->SetRelativeLocation(FVector(0, 0, 100));

	TurretVision->SetSphereRadius(0);
}

void ATurret::BeginPlay(){
	Super::BeginPlay();

	InteractableComp->AddInteractable(TurretBase);
	InteractableComp->AddInteractable(TurretPillar);
	InteractableComp->AddInteractable(TurretHead);

	FOnTimelineFloat UpdateEvent;
	FOnTimelineEvent FinishEvent;

	UpdateEvent.BindDynamic(this, &ATurret::RotateToTarget);
	FinishEvent.BindDynamic(this, &ATurret::EndRotate);
	RotateTimeline.AddInterpFloat(ZeroToOneCurve, UpdateEvent);
	RotateTimeline.SetTimelineFinishedFunc(FinishEvent);

	TurretVision->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnReachVision);
	TurretVision->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnLeaveVision);
}

void ATurret::Tick(float deltaTime){
	Super::Tick(deltaTime);

	RotateTimeline.TickTimeline(deltaTime);
}

void ATurret::LookAtTarget(){
	SelectTarget();
	CurrentYawRotation = TurretPillar->GetComponentRotation().Yaw;
	CurrentPitchRotation = TurretHead->GetComponentRotation().Pitch;
	RotateTimeline.PlayFromStart();
}

void ATurret::RotateToTarget(float Alpha){
	if (!IsValid(CurrentTarget)){
		RotateTimeline.Stop();
		EndRotate();
		return;
	}

	AILookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTarget->GetActorLocation());
	FRotator PillarRotator = FRotator::ZeroRotator;
	//FRotator HeadRotator = FRotator::ZeroRotator;

	PillarRotator.Yaw = FMath::Lerp(CurrentYawRotation, AILookAtRotation.Yaw, Alpha);
	//HeadRotator.Pitch = FMath::Lerp(CurrentPitchRotation, AILookAtRotation.Pitch, Alpha);

	TurretPillar->SetWorldRotation(PillarRotator);
	//TurretHead->SetRelativeRotation(HeadRotator);
}

void ATurret::EndRotate_Implementation(){}

void ATurret::ReceiveGlitchUpgrade(){
	AttackRate = CurrentData->GlitchAttackRate;
	Damages = CurrentData->GlitchDamages;
	AttackRange = CurrentData->GlitchAttackRange;

	RotateTimeline.SetPlayRate(1/AttackRate);
	TurretVision->SetSphereRadius(AttackRange, true);

	Super::ReceiveGlitchUpgrade();
}

void ATurret::SetMesh(){
	Super::SetMesh();

	TurretBase->SetStaticMesh(Cast<UStaticMesh>(CurrentData->MeshList[0]));
	TurretPillar->SetStaticMesh(Cast<UStaticMesh>(CurrentData->MeshList[1]));
	TurretHead->SetSkeletalMesh(Cast<USkeletalMesh>(CurrentData->MeshList[2]), true);
	TurretHead->PlayAnimation(IdleAnimation, true);

	TurretBase->SetVectorParameterValueOnMaterials("CrystalColor", FVector(CurrentData->CrystalColor));
	TurretPillar->SetVectorParameterValueOnMaterials("CrystalColor", FVector(CurrentData->CrystalColor));
	TurretHead->SetVectorParameterValueOnMaterials("CrystalColor", FVector(CurrentData->CrystalColor));

	TurretVision->SetSphereRadius(AttackRange, true);
}

void ATurret::SetData(UPlacableActorData* NewData){
	Super::SetData(NewData);

	const UTurretData* Data = Cast<UTurretData>(NewData);
	Damages = Data->Damages;
	AttackRate = Data->AttackRate/2;
	RotateTimeline.SetPlayRate(1/AttackRate);

	CanSeeThroughWalls = Data->CanSeeThroughWalls;
	FocusMethod = Data->FocusMethod;
}

void ATurret::Appear(const bool ReverseEffect){
	FullMesh = Cast<UStaticMeshComponent>(AddComponentByClass(UStaticMeshComponent::StaticClass(), true, GetActorTransform(), false));

	FullMesh->SetStaticMesh(CurrentData->FullMesh);

	for(int i = 0; i < FullMesh->GetNumMaterials(); i++){
		FullMesh->SetMaterial(i, CurrentData->AppearanceMaterial);
	}

	if(ReverseEffect){
		TurretBase->SetVisibility(false);
		TurretPillar->SetVisibility(false);
		TurretHead->SetVisibility(false);
	}

	Super::Appear(ReverseEffect);
}

void ATurret::FadeIn(float Alpha){
	Super::FadeIn(Alpha);

	FullMesh->SetScalarParameterValueOnMaterials("PercentageApparition", Alpha);
}

void ATurret::EndAppearance(){
	Super::EndAppearance();

	FullMesh->DestroyComponent();
}

void ATurret::CanAttack(){
	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;

	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), TurretHead->GetComponentLocation(), GetFirstAI()->GetActorLocation(), UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, true, FLinearColor::Red, FLinearColor::Green, 0.1f);

	if (Hit.GetActor()->IsA(AMainAICharacter::StaticClass())){
		GetWorldTimerManager().ClearTimer(CanAttackTimer);
		CurrentTarget = Hit.GetActor();
		Attack();
	}
}

void ATurret::Attack_Implementation(){
	SelectTarget();
	Super::Attack_Implementation();
}

void ATurret::EndAttack() {
	if (DoesAIListContainSomething()) {
		SelectTarget();
		if (!CanSeeThroughWalls) {
			GetWorld()->GetTimerManager().SetTimer(CanAttackTimer, [&]() {
				CanAttack();
				}, 0.1f, true);
		}else {
			Attack();
		}
	} else {
		FinishAttacking();
	}
}

void ATurret::FinishAttacking_Implementation(){}

void ATurret::Shoot_Implementation(){}

AActor* ATurret::GetFirstAI(){
	return UUsefullFunctions::SortActorsByDistanceToActor(GetSortedAIList(), Cast<AActor>(Nexus))[0];
}

AActor* ATurret::GetMidAI(){
	if (GetSortedAIList().Num() == 0) {
		return nullptr;
	}

	return UUsefullFunctions::SortActorsByDistanceToActor(GetSortedAIList(), Cast<AActor>(Nexus))[GetSortedAIList().Num() / 2];
}

AActor* ATurret::GetLastAI(){
	if (GetSortedAIList().Num() == 0) {
		return nullptr;
	}

	return UUsefullFunctions::SortActorsByDistanceToActor(GetSortedAIList(), Cast<AActor>(Nexus))[GetSortedAIList().Num() - 1];
}

void ATurret::SelectTarget(){
	switch (FocusMethod) {
	case FFocusMethod::FirstTarget:
		CurrentTarget = GetFirstAI();
		break;
	case FFocusMethod::MidTarget:
		CurrentTarget = GetMidAI();
		break;
	case FFocusMethod::LastTarget:
		CurrentTarget = GetLastAI();
		break;
	}
}

TArray<AActor*> ATurret::GetSortedAIList() const{
	TArray<AMainAICharacter*> AIArray = AIList.Array();
	TArray<AActor*> ActorArray;

	for (int i = 0; i < AIArray.Num(); i++) {
		ActorArray.Add(AIArray[i]);
	}

	return ActorArray;
}

bool ATurret::DoesAIListContainSomething() const{
	return AIList.Num() > 0;
}

void ATurret::OnReachVision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnReachVision(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (AIList.Num() == 1) {

		if (!CanSeeThroughWalls) {
			GetWorld()->GetTimerManager().SetTimer(CanAttackTimer, [&]() {
				CanAttack();
			}, 0.1f, true);
		} else {
			Attack();
		}
	}
}

void ATurret::OnLeaveVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnLeaveVision(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

	if (AIList.Num() == 0) {

		if (!CanSeeThroughWalls) {
			GetWorld()->GetTimerManager().ClearTimer(CanAttackTimer);
		} 
	}
}

void ATurret::SetObjectMaterial(UMaterialInterface* NewMaterial){
	Super::SetObjectMaterial(NewMaterial);

	TurretPillar->SetMaterial(0, NewMaterial);
	Cast<UMeshComponent>(TurretHead)->SetMaterial(0, NewMaterial);
}