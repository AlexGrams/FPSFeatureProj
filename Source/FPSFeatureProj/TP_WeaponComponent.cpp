// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "FPSFeatureProjCharacter.h"
#include "FPSFeatureProjProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	IsHitscan = false;
	HitscanRange = 999999.0f;
	MaxAmmo = 5;
	CurrentAmmo = MaxAmmo;

	// Automatic weapon settings
	IsAutomatic = false;
	FireInterval = 0.2f;
	FireInputHeld = false;
	CanFire = true;
}

void UTP_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Fires continuously if automatic, or fires weapon once if not.
void UTP_WeaponComponent::OnFirePressed()
{
	if (CanFire)
		Fire();

	if (IsAutomatic)
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
}

// Reset automatic fire variables 
void UTP_WeaponComponent::OnFireReleased()
{
	FireInputHeld = false;
}

void UTP_WeaponComponent::Fire()
{
	if(Character == nullptr || Character->GetController() == nullptr || CurrentAmmo <= 0)
	{
		return;
	}

	CurrentAmmo -= 1;

	// Try to fire a projectile
	if (IsHitscan)
	{
		// Hitscan weapon
		FHitResult OutHit = FHitResult();
		FVector Start = Character->GetFirstPersonCameraComponent()->GetComponentLocation();
		FVector End = Start + Character->GetFirstPersonCameraComponent()->GetForwardVector() * HitscanRange;

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
		// Projectile weapon
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ActorSpawnParams.Instigator = Character;
	
			// Spawn the projectile at the muzzle
			AFPSFeatureProjProjectile* SpawnedProjectile = World->SpawnActor<AFPSFeatureProjProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (SpawnedProjectile->Implements<UProjectileInterface>())
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
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

// To be called by a Timer on automatic weapons. Clears timer if Fire input is no longer held.
void UTP_WeaponComponent::AutoFireFunction()
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

// Reload weapon
void UTP_WeaponComponent::Reload()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("TODO: reloading action")));
	CurrentAmmo = MaxAmmo;
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Unequip();

	// TODO: See if code is equivalent to calling Unequip()
	//if(Character != nullptr)
	//{
	//	// Unregister from the OnUseItem Event
	//	Character->OnUseItem.RemoveDynamic(this, &UTP_WeaponComponent::Fire);

	//	// Unregister from Reload event
	//	Character->OnReload.RemoveDynamic(this, &UTP_WeaponComponent::Reload);
	//}
}

void UTP_WeaponComponent::AttachWeapon(AFPSFeatureProjCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if(Character != nullptr)
	{
		// Attach the weapon to the First Person Character
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		GetOwner()->AttachToComponent(Character->GetMesh1P(),AttachmentRules, FName(TEXT("GripPoint")));

		// Add WeaponComponent root to Character's array
		Character->PickUpWeapon(GetOwner());
	}
}

void UTP_WeaponComponent::Equip()
{
	if (Character == nullptr)
	{
		return;
	}

	// Unhide this weapon
	GetOwner()->SetActorHiddenInGame(false);

	// Register with weapon-related events
	Character->OnUseItem.AddDynamic(this, &UTP_WeaponComponent::OnFirePressed);
	Character->OnUseItemRelease.AddDynamic(this, &UTP_WeaponComponent::OnFireReleased);
	Character->OnReload.AddDynamic(this, &UTP_WeaponComponent::Reload);
}

void UTP_WeaponComponent::Unequip()
{
	// Hide this weapon
	GetOwner()->SetActorHiddenInGame(true);

	// Unregister with weapon-related events
	if (Character != nullptr)
	{
		if (Character->OnUseItem.Contains(this, FName(TEXT("OnFirePressed"))))
		{
			Character->OnUseItem.RemoveDynamic(this, &UTP_WeaponComponent::OnFirePressed);
		}
		if (Character->OnUseItemRelease.Contains(this, FName(TEXT("OnFireReleased"))))
		{
			Character->OnUseItemRelease.RemoveDynamic(this, &UTP_WeaponComponent::OnFireReleased);
		}
		if (Character->OnReload.Contains(this, FName(TEXT("Reload"))))
		{
			Character->OnReload.RemoveDynamic(this, &UTP_WeaponComponent::Reload);
		}

		// Stop automatic firing if Fire input is held down.
		if (IsAutomatic && AutoFireHandle.IsValid() && Character->GetWorldTimerManager().IsTimerActive(AutoFireHandle))
			Character->GetWorldTimerManager().ClearTimer(AutoFireHandle);
	}
}

