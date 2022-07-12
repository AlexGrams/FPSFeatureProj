// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "InteractableInterface.h"
#include "ButtonComponent.generated.h"

/**
 * Component for buttons. Has a sphere collider. Interaction causes toggle on/off.
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class FPSFEATUREPROJ_API UButtonComponent : public USphereComponent, public IInteractableInterface
{
	GENERATED_BODY()

protected:
	// --- Variables ---
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bOn;

	// Toggleable linked to this button. Must have ToggleableInterface
	UPROPERTY(EditInstanceOnly, Category = "Interaction")
	AActor* Toggleable;

	
public:
	// --- Functions ---
	UButtonComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract();
};
