// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlateComponent.h"
#include "ToggleableInterface.h"

// Sets default values for this component's properties
UPressurePlateComponent::UPressurePlateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bOn = false;
}

// Called when first item steps on/off of Pressure Plate
void UPressurePlateComponent::OnInteract_Implementation()
{
	// TODO: Store On property on Door itself, so that multiple buttons can operate the same door.
	bOn = !bOn;

	if (Toggleable)
	{
		// To call C++ interface implemented on a BP, use UInterfaceName instead of IInterfaceName
		if (Toggleable->Implements<UToggleableInterface>())
		{
			IToggleableInterface* Interface = Cast<IToggleableInterface>(Toggleable);
			Interface->Execute_Toggle(Toggleable, bOn);
		}
	}
}

