// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableObject/Turret.h"
#include "Components/InteractableComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI/MainAICharacter.h"
#include "PlacableObject/TurretData.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"

ATurret::ATurret() {
	PrimaryActorTick.bCanEverTick = true;

	TurretPillar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pillar"));
	TurretPillar->SetupAttachment(BaseMesh);
	
	TurretHead = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	TurretHead->SetupAttachment(TurretPillar);
	TurretHead->SetRelativeLocation(FVector(0, 0, 100));

	TurretRadius = CreateDefaultSubobject<USphereComponent>(TEXT("TurretRadius"));
	TurretRadius->SetupAttachment(BaseMesh);
	TurretRadius->SetCollisionResponseToAllChannels(ECR_Overlap);
	TurretRadius->SetSphereRadius(100, false);

	BaseMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	TurretPillar->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	TurretHead->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	TurretRadius->SetCanEverAffectNavigation(false);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprint/Curves/FC_ZeroToOneCurve"));
	check(Curve.Succeeded());

	ZeroToOneCurve = Curve.Object;
}

void ATurret::BeginPlay(){
	Super::BeginPlay();

	InteractableComp->AddInteractable(TurretPillar);
	InteractableComp->AddInteractable(TurretHead);

	FOnTimelineFloat UpdateEvent;
	FOnTimelineEvent FinishEvent;

	UpdateEvent.BindDynamic(this, &ATurret::RotateToTarget);
	FinishEvent.BindDynamic(this, &ATurret::EndRotate);
	RotateTimeline.AddInterpFloat(ZeroToOneCurve, UpdateEvent);
	RotateTimeline.SetTimelineFinishedFunc(FinishEvent);
}

void ATurret::Tick(float deltaTime){
	Super::Tick(deltaTime);

	RotateTimeline.TickTimeline(deltaTime);
}

void ATurret::LookAtTarget(){
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

void ATurret::GlitchUpgrade(){
	Damages = Cast<UTurretData>(CurrentData)->UpgradedGlitchDamages;
	
	Super::GlitchUpgrade();
}

void ATurret::SetMesh(){
	Super::SetMesh();

	TurretPillar->SetStaticMesh(Cast<UStaticMesh>(CurrentData->MeshList[1]));
	TurretHead->SetSkeletalMesh((Cast<USkeletalMesh>(CurrentData->MeshList[2])), true);
}

void ATurret::SetData(UPlacableActorData* NewData){
	Super::SetData(NewData);
	
	UTurretData* Data = Cast<UTurretData>(NewData);
	Damages = Data->Damages;
	FireRate = Data->FireRate;
	Radius = Data->TurretRadius;
}

void ATurret::CanAttack(){
	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;

	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), TurretHead->GetComponentLocation(), GetFirstAI()->GetActorLocation(), UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true, FLinearColor::Red, FLinearColor::Green, 0.1f);

	if (Hit.GetActor()->IsA(AMainAICharacter::StaticClass())){
		GetWorldTimerManager().ClearTimer(CanAttackTimer);
		CurrentTarget = Hit.GetActor();
		Attack();
	}
}

void ATurret::Attack_Implementation(){}

AActor* ATurret::GetFirstAI(){
	TArray<AMainAICharacter*> AIArray = AIList.Array();
	TArray<AActor*> ActorArray;

	for (int i = 0; i < AIArray.Num(); i++) {
		ActorArray.Add(AIArray[i]);
	}

	return UUsefullFunctions::SortActorsByDistanceToActor(ActorArray, Cast<AActor>(Nexus))[0];
}

bool ATurret::DoesAIListContainSomething(){
	return AIList.Num() > 0;
}

void ATurret::BeginOverlap_Implementation(AActor* OverlappedActor){
	if (OverlappedActor->IsA(AMainAICharacter::StaticClass())) {
		AIList.Add(Cast<AMainAICharacter>(OverlappedActor));
	}
}

void ATurret::EndOverlap_Implementation(AActor* OverlappedActor){
	if (OverlappedActor->IsA(AMainAICharacter::StaticClass())) {
		AIList.Remove(Cast<AMainAICharacter>(OverlappedActor));
	}
}

void ATurret::SetObjectMaterial(UMaterialInterface* NewMaterial){
	Super::SetObjectMaterial(NewMaterial);

	TurretPillar->SetMaterial(0, NewMaterial);
	Cast<UMeshComponent>(TurretHead)->SetMaterial(0, NewMaterial);
}
