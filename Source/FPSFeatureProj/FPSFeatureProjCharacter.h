// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TP_WeaponComponent.h"
#include "FPSFeatureProjCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

// Declaration of the delegate that will be called when the Primary Action is triggered
// It is declared as dynamic so it can be accessed also in Blueprints
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);
// Delegate for releasing held input
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItemRelease);
// Similar delegate for reloading
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReload);

UCLASS(config=Game)
class AFPSFeatureProjCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

protected:
	// If true, prevents movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool IsDodging;

	// How long player dodges for
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DodgeTime;

	// Value to scale default movement speed by when dodging
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DodgeSpeedMultiplier;

	// Value to scale default movement acceleration by when dodging
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DodgeAccelerationMultiplier;

	// Used through duration of a dodge
	FTimerHandle DodgeTimerHandle;

	// Stored dodge direction
	FVector DodgeDirection;

	float DefaultMaxWalkSpeed;
	float DefaultMaxAcceleration;

public:
	AFPSFeatureProjCharacter();

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnUseItem;

	// Delegate for releasing primary action input
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItemRelease OnUseItemRelease;

	// Delegate for reloading
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnReload OnReload;
protected:

	// Tick function
	virtual void Tick(float DeltaSeconds);
	
	/** Fires a projectile. */
	void OnPrimaryAction();

	// Player released PrimaryAction input
	void OnPrimaryActionRelease();

	// Reload weapon
	void OnReloadAction();

	// Weapon switching
	void OnNextWeapon();
	void OnPreviousWeapon();

	// Dodging
	void OnDodge();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

	// Weapon selection array
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	TArray<AActor*> Weapons;

	// Current WeaponComponent
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	UTP_WeaponComponent* CurrentWeapon;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	int CurrentWeaponIndex;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	// Pick up new weapon
	void PickUpWeapon(AActor* NewWeapon);

	// Add weapon
	void AddWeapon(AActor* NewWeapon);

	// Swap to weapon using UTP_WeaponComponent pointer or weapon index
	void SwapToWeapon(UTP_WeaponComponent* NewEquipedWeapon);
	void SwapToWeapon(int WeaponIndex);

	// Reset properties that were changed while dodging
	UFUNCTION()
	void StopDodging();

};

