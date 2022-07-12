// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableInterface.h"
#include "PressurePlateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSFEATUREPROJ_API UPressurePlateComponent : public UActorComponent, public IInteractableInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPressurePlateComponent();

	// --- Functions ---

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract();

protected:
	// --- Variables ---

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bOn;

	// Toggleable linked to this button. Must have ToggleableInterface
	UPROPERTY(EditInstanceOnly, Category = "Interaction")
	AActor* Toggleable;
		
};
