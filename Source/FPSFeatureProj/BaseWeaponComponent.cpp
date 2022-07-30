// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponComponent.h"
#include "FPSFeatureProjProjectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBaseWeaponComponent::UBaseWeaponComponent()
{

}


// Called when the game starts
void UBaseWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = nullptr;
}

void UBaseWeaponComponent::Fire()
{
	// TODO: Better pointer validation checks
	if (!IsValid(Character) || Character == nullptr || Character->GetController() == nullptr || CurrentAmmo <= 0)
	{
		return;
	}

	CurrentAmmo -= 1;

	if (IsHitscan)
	{
		// Firing for Hitscan weapon

		FHitResult OutHit = FHitResult();
		FVector Start = GetHitscanStart();//Character->GetFirstPersonCameraComponent()->GetComponentLocation();
		FVector End = Start + HitscanRange * GetHitscanDirection();//Character->GetFirstPersonCameraComponent()->GetForwardVector();

		// List of types that hitscan bullet can hit
		FCollisionObjectQueryParams ObjectQueryParams = FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic) | ECC_TO_BITFIELD(ECC_Pawn) | ECC_TO_BITFIELD(ECC_PhysicsBody));
		// Line Trace parameters
		FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("Hitscan")), true, Character);
		bool Result = GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, ObjectQueryParams, Params);

		if (Result)
		{
			// Actual damage that was applied
			float TakenDamage = 0.0f;

			// TODO: Find best place to set these
			AActor* DamagedActor = OutHit.GetActor();
			AController* EventInstigator = Character->GetController();
			AActor* DamageCauser = Character;
			TSubclassOf<UDamageType> DamageTypeClass = UDamageType::StaticClass();

			if (!DamagedActor)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Unknown hit object")));
			}
			else if (Damage != 0.f)
			{
				// make sure we have a good damage type
				TSubclassOf<UDamageType> const ValidDamageTypeClass = DamageTypeClass ? DamageTypeClass : TSubclassOf<UDamageType>(UDamageType::StaticClass());
				FDamageEvent DamageEvent(ValidDamageTypeClass);
				TakenDamage = DamagedActor->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Not valid hit")));
		}
	}
	else if (ProjectileClass != nullptr)
	{
		// Firing for Projectile weapon

		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			// TODO: Another null check for some reason
			if (!IsValid(Character))
			{
				return;
			}

			const FRotator SpawnRotation = GetProjectileRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ActorSpawnParams.Instigator = Character;

			// Spawn the projectile at the muzzle
			AFPSFeatureProjProjectile* SpawnedProjectile = World->SpawnActor<AFPSFeatureProjProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (IsValid(SpawnedProjectile) && SpawnedProjectile->Implements<UProjectileInterface>())
			{
				IProjectileInterface* Interface = Cast<IProjectileInterface>(SpawnedProjectile);
				AController* EventInstigator = Character->GetController();
				AActor* DamageCauser = Character;

				Interface->Execute_SetProperties(SpawnedProjectile, Damage, EventInstigator, DamageCauser);
			}
		}
	}

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	PlayFireAnimation();
}

// Reload weapon
void UBaseWeaponComponent::Reload()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("TODO: reloading action")));
	CurrentAmmo = MaxAmmo;
}

// To be called by a Timer on automatic weapons. Clears timer if Fire input is no longer held.
void UBaseWeaponComponent::AutoFireFunction()
{
	if (Character == nullptr)
	{
		return;
	}

	if (FireInputHeld)
	{
		Fire();
	}
	else
	{
		CanFire = true;
		Character->GetWorldTimerManager().ClearTimer(AutoFireHandle);
	}
}

void UBaseWeaponComponent::StartAutoFire()
{
	// Bind AutoFireFunction if it hasn't been bound already.
	if (!AutoFireDelegate.IsBound())
	{
		AutoFireDelegate.BindUFunction(this, TEXT("AutoFireFunction"));
	}

	FireInputHeld = true;
	CanFire = false;

	// Set timer only if it is not already running
	if (Character != nullptr)
	{
		if (!Character->GetWorldTimerManager().IsTimerActive(AutoFireHandle))
		{
			Character->GetWorldTimerManager().SetTimer(AutoFireHandle, AutoFireDelegate, FireInterval, true);
		}
	}
}

void UBaseWeaponComponent::EndAutoFire()
{
	FireInputHeld = false;
}

void UBaseWeaponComponent::PlayFireAnimation()
{
	return;
}
