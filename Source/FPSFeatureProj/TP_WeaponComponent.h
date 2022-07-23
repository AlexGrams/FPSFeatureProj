// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TP_WeaponComponent.generated.h"

class AFPSFeatureProjCharacter;

// Modified version of base Unreal FPS project class
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSFEATUREPROJ_API UTP_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// --- Variables ---
	// TODO: Convert component class to own Actor-derived class that all weapons can inherit from.
	/** Projectile class to spawn */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=Projectile)
	TSubclassOf<class AFPSFeatureProjProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	// True if gun does not fire a physics projectile.
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Weapon)
	bool IsHitscan;

	// Can weapon be fired by holding down fire input?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Weapon)
	bool IsAutomatic;

	// How far a hitscan weapon can shoot
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Weapon)
	float HitscanRange;
	
	// Weapon damage
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Weapon)
	float Damage;

	// Max ammo count
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Weapon)
	int MaxAmmo;

	// Current ammo count
	UPROPERTY(BlueprintReadWrite, Category = Weapon)
	int CurrentAmmo;

	// Seconds between firing
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Weapon)
	float FireInterval;

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

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	// Reload weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

protected:
	// --- Variables ---

	// True if player is holding down the "Fire" input
	bool FireInputHeld;

	// True if weapon can be fired currently
	bool CanFire;

	// For automatic weapons
	FTimerHandle AutoFireHandle;
	FTimerDelegate AutoFireDelegate;


	// --- Functions ---

	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
		
	// Functions to be bound for player pressing/releasing fire input
	UFUNCTION()
	void OnFirePressed();
	UFUNCTION()
	void OnFireReleased();

	// Called on a timer for repeating weapons
	UFUNCTION()
	void AutoFireFunction();

private:
	/** The Character holding this weapon*/
	AFPSFeatureProjCharacter* Character;
};
