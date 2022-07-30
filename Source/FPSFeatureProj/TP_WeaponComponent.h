// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseWeaponComponent.h"
#include "TP_WeaponComponent.generated.h"

class AFPSFeatureProjCharacter;

// Modified version of base Unreal FPS project class
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSFEATUREPROJ_API UTP_WeaponComponent : public UBaseWeaponComponent
{
	GENERATED_BODY()

public:

	// --- Functions ---

	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	UFUNCTION()
	void BeginPlay();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AFPSFeatureProjCharacter* TargetCharacter);

	// Equip this weapon, allowing player to fire it
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Equip();

	// Unequip weapon, hiding it and disabling fire functionality
	UFUNCTION(Blueprintcallable, Category = "Weapon")
	void Unequip();

protected:
	// --- Functions ---

	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
		
	// Functions to be bound for player pressing/releasing fire input
	UFUNCTION()
	void OnFirePressed();
	UFUNCTION()
	void OnFireReleased();

	// Get the origin point of a hitscan line trace
	virtual FVector GetHitscanStart() const override; 

	// Get the direction of a hitscan line trace
	virtual FVector GetHitscanDirection() const override;

	// Get spawn rotation for a projectile
	virtual FRotator GetProjectileRotation() const override;

	// Play first person firing animation
	virtual void PlayFireAnimation() override;

private:
	/** The AFPSFeatureProjCharacter holding this weapon. Separate from base class Character*/
	AFPSFeatureProjCharacter* FPSCharacter;
};
