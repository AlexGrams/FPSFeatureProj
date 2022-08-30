// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "BaseWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSFEATUREPROJ_API UBaseWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// --- Variables ---
	// TODO: Convert component class to own Actor-derived class that all weapons can inherit from.

	/** Projectile class to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<class AFPSFeatureProjProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	// True if gun does not fire a physics projectile.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	bool IsHitscan;

	// Can weapon be fired by holding down fire input?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Weapon)
	bool bIsAutomatic;

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

	// Sets default values for this component's properties
	UBaseWeaponComponent();

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire();

	// Reload weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

	// Start automatic firing. Weapon will not stop firing until EndAutoFire() is called.
	// bIsAutomatic should be checked before this function is called.
	void StartAutoFire();

	// Ends automatic firing.
	void EndAutoFire();

protected:
	// --- Variables
	
	// The character holding this weapon
	ACharacter* Character;

	// True if weapon can be fired currently
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	bool bCanFire;

	// Speed at which this Weapon's projectile is spawned at, if it is a projectile-type weapon. Use a negative value to spawn with the Projectile's default speed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	float ProjectileInitialSpeed;

	// For automatic weapons
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	FTimerHandle AutoFireHandle;

	FTimerDelegate AutoFireDelegate;

	// True if weapon holder is continuously attempting to fire weapon
	bool bFireInputHeld;

	// The Projectile that was just spawned. Used for referring to a Projectile the same frame it is created.
	AFPSFeatureProjProjectile* SpawnedProjectile;


	// --- Functions ---

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called on a timer for repeating weapons
	UFUNCTION()
	void AutoFireFunction();

	// Get the origin point of a hitscan line trace
	virtual FVector GetHitscanStart() const { return Character->GetActorLocation(); }
		
	// Get the direction of a hitscan line trace
	virtual FVector GetHitscanDirection() const { return Character->GetActorForwardVector(); }

	// Get spawn rotation for a projectile
	virtual FRotator GetProjectileRotation() const { return IsValid(Character) ? Character->GetActorRotation() : FRotator::ZeroRotator; }

	// Play a firing animation, if specified
	virtual void PlayFireAnimation();

	// Uses a line trace to fire at and damage a target. Called when a hitscan weapon is fired, or can be called separately from regular Fire() function.
	void FireHitscanWeapon();

	// Damage a target that was hit by this Weapon. Can optionally specify the DamageType.
	void DamageTarget(AActor* Target, TSubclassOf<UDamageType> DamageTypeClass = UDamageType::StaticClass());
};
