// Fill out your copyright notice in the Description page of Project Settings.

#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "Components/InteractableComponent.h"

UInteractableComponent::UInteractableComponent(){
	PrimaryComponentTick.bCanEverTick = false;
}


void UInteractableComponent::BeginPlay(){
	Super::BeginPlay();

	if (bOutlineFeedback){
		//OnFeedback.Bind()
	}
}

bool UInteractableComponent::CheckComponent(UPrimitiveComponent* ComponentToCheck){
	return InteractableComponentSet.Contains(ComponentToCheck);
}

#pragma region Add/Remove Interactable

void UInteractableComponent::AddInteractable(UPrimitiveComponent* ComponentToAdd){
	InteractableComponentSet.Add(ComponentToAdd);
}

void UInteractableComponent::AddInteractables(TArray<UPrimitiveComponent*> ComponentsToAdd){
	InteractableComponentSet = TSet<UPrimitiveComponent*>(ComponentsToAdd);
}

void UInteractableComponent::RemoveInteractable(UPrimitiveComponent* ComponentToRemove){
	InteractableComponentSet.Remove(ComponentToRemove);
}

void UInteractableComponent::RemoveInteractables(TArray<UPrimitiveComponent*> ComponentsToRemove){
	for (int i = 0; i < sizeof(ComponentsToRemove); i++) {
		RemoveInteractable(ComponentsToRemove[i]);
	}
}

#pragma endregion

#pragma region Interaction & feedback

void UInteractableComponent::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	OnInteract.Broadcast(MainPlayerController, MainPlayer);
}

void UInteractableComponent::Feedback(){
	OnFeedback.Broadcast();
	OutlineFeedback(true);
}

void UInteractableComponent::Unfeedback(){
	OnUnfeedback.Broadcast();
	OutlineFeedback(false);
}

void UInteractableComponent::OutlineFeedback(bool bOutline){
	// crée 16 élements chai pas pourquoi
	TArray<UPrimitiveComponent*> interactableList = InteractableComponentSet.Array();
	
	//fonctionne juste avec le 1er element de la liste
	UUsefullFunctions::OutlineComponent(bOutline, interactableList[0]);

	// cause un crash parce qu'il essaie d'accéder aux truc qui existent pas
	/*for (int i = 0; i < sizeof(interactableList); i++) {
		UUsefullFunctions::OutlineComponent(bOutline, interactableList[i]);
	}*/
}

#pragma endregion
