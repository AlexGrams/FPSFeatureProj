// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponComponent.h"
#include "FPSFeatureProjProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "CritDamageType.h"

// Custom collision channels (For future reference. Some might not be used.)
#define ECC_Projectile			ECC_GameTraceChannel1
#define ECC_EnemyProjectile		ECC_GameTraceChannel2
#define ECC_Enemy				ECC_GameTraceChannel3
#define WeakPointTag			TEXT("WeakPoint")

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
	if (!IsValid(Character) || Character == nullptr || Character->GetController() == nullptr || (!InfiniteAmmo && CurrentAmmo <= 0))
	{
		return;
	}

	if (!InfiniteAmmo)
	{
		CurrentAmmo -= 1;
	}

	if (IsHitscan)
	{
		FireHitscanWeapon();
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
			SpawnedProjectile = World->SpawnActor<AFPSFeatureProjProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (IsValid(SpawnedProjectile) && SpawnedProjectile->Implements<UProjectileInterface>())
			{
				IProjectileInterface* Interface = Cast<IProjectileInterface>(SpawnedProjectile);
				AController* EventInstigator = Character->GetController();
				AActor* DamageCauser = Character;

				Interface->Execute_SetProperties(SpawnedProjectile, Damage, EventInstigator, DamageCauser, ProjectileInitialSpeed);
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

void UBaseWeaponComponent::FireHitscanWeapon()
{
	// Firing for Hitscan weapon

	FHitResult OutHit = FHitResult();
	FVector Start = GetHitscanStart();//Character->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector End = Start + HitscanRange * GetHitscanDirection();//Character->GetFirstPersonCameraComponent()->GetForwardVector();

	// List of types that hitscan bullet can hit
	FCollisionObjectQueryParams ObjectQueryParams = FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic) |
		ECC_TO_BITFIELD(ECC_Pawn) | ECC_TO_BITFIELD(ECC_PhysicsBody) | ECC_TO_BITFIELD(ECC_Enemy));
	// Line Trace parameters
	FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("Hitscan")), true, Character);
	bool Result = GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, ObjectQueryParams, Params);

	if (Result)
	{
		if (OutHit.Component.IsValid() && OutHit.Component.Get()->ComponentHasTag(WeakPointTag))
		{
			DamageTarget(OutHit.GetActor(), UCritDamageType::StaticClass());
		}
		else
		{
			DamageTarget(OutHit.GetActor());
		}
	}
}

void UBaseWeaponComponent::DamageTarget(AActor* Target, TSubclassOf<UDamageType> DamageTypeClass)
{
	// Actual damage that was applied
	float TakenDamage = 0.0f;

	if (!IsValid(Target))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Unknown weapon target")));
	}
	else if (Damage != 0.f)
	{
		// make sure we have a good damage type
		TSubclassOf<UDamageType> const ValidDamageTypeClass = DamageTypeClass ? DamageTypeClass : TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);
		TakenDamage = Target->TakeDamage(Damage, DamageEvent, Character->GetController(), Character);
	}
}

// Reload weapon
void UBaseWeaponComponent::Reload()
{
	CurrentAmmo = MaxAmmo;
}

// To be called by a Timer on automatic weapons. Clears timer if Fire input is no longer held.
void UBaseWeaponComponent::AutoFireFunction()
{
	if (Character == nullptr)
	{
		return;
	}

	if (bFireInputHeld)
	{
		Fire();
	}
	else
	{
		bCanFire = true;
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

	bFireInputHeld = true;
	bCanFire = false;

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
	bFireInputHeld = false;
}

void UBaseWeaponComponent::PlayFireAnimation()
{
	return;
}
