// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonComponent.h"
#include "ToggleableInterface.h"

UButtonComponent::UButtonComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bOn = false;
	SphereRadius = 120.0f;
}

// Called when Interact input is pressed on this Button.
void UButtonComponent::OnInteract_Implementation()
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

	//if (Toggleable)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Link toggleable")));
	//	IToggleableInterface* Interface = Cast<IToggleableInterface>(Toggleable);
	//	if (Interface)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Interface thing")));
	//		Interface->Execute_Toggle(Toggleable, bOn);
	//	}
	//}
}
