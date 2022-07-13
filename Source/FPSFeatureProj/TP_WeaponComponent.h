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
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Gameplay)
	bool IsHitscan;

	// How far a hitscan weapon can shoot
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Gameplay)
	float HitscanRange;

	// Max ammo count
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Gameplay)
	int MaxAmmo;

	// Current ammo count
	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	int CurrentAmmo;

	// --- Functions ---
	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	UFUNCTION()
	void BeginPlay();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AFPSFeatureProjCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	// Reload weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
		

private:
	/** The Character holding this weapon*/
	AFPSFeatureProjCharacter* Character;
};
