// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "FPSFeatureProjCharacter.h"
#include "FPSFeatureProjProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
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
	Damage = 10.0f;
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

FVector UTP_WeaponComponent::GetHitscanStart() const
{ 
	return FPSCharacter->GetFirstPersonCameraComponent()->GetComponentLocation(); 
} 

FVector UTP_WeaponComponent::GetHitscanDirection() const
{ 
	return FPSCharacter->GetFirstPersonCameraComponent()->GetForwardVector(); 
}

FRotator UTP_WeaponComponent::GetProjectileRotation() const
{
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	return PlayerController->PlayerCameraManager->GetCameraRotation();
}

// Fires continuously if automatic, or fires weapon once if not.
void UTP_WeaponComponent::OnFirePressed()
{
	if (CanFire)
		Fire();

	if (IsAutomatic)
	{
		StartAutoFire();
	}
}

// Reset automatic fire variables 
void UTP_WeaponComponent::OnFireReleased()
{
	EndAutoFire();
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
	FPSCharacter = TargetCharacter;

	if(Character != nullptr)
	{
		// Attach the weapon to the First Person Character
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		GetOwner()->AttachToComponent(FPSCharacter->GetMesh1P(),AttachmentRules, FName(TEXT("GripPoint")));

		// Add WeaponComponent root to Character's array
		FPSCharacter->PickUpWeapon(GetOwner());
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
	FPSCharacter->OnUseItem.AddDynamic(this, &UTP_WeaponComponent::OnFirePressed);
	FPSCharacter->OnUseItemRelease.AddDynamic(this, &UTP_WeaponComponent::OnFireReleased);
	FPSCharacter->OnSecondaryAction.AddDynamic(this, &UTP_WeaponComponent::OnSecondaryPressed);
	FPSCharacter->OnSecondaryActionRelease.AddDynamic(this, &UTP_WeaponComponent::OnSecondaryReleased);
	FPSCharacter->OnReload.AddDynamic(this, &UTP_WeaponComponent::Reload);
}

void UTP_WeaponComponent::Unequip()
{
	// Hide this weapon
	GetOwner()->SetActorHiddenInGame(true);

	// Unregister with weapon-related events
	if (FPSCharacter != nullptr)
	{
		if (FPSCharacter->OnUseItem.Contains(this, FName(TEXT("OnFirePressed"))))
		{
			FPSCharacter->OnUseItem.RemoveDynamic(this, &UTP_WeaponComponent::OnFirePressed);
		}
		if (FPSCharacter->OnUseItemRelease.Contains(this, FName(TEXT("OnFireReleased"))))
		{
			FPSCharacter->OnUseItemRelease.RemoveDynamic(this, &UTP_WeaponComponent::OnFireReleased);
		}

		if (FPSCharacter->OnSecondaryAction.Contains(this, FName(TEXT("OnSecondaryPressed"))))
		{
			FPSCharacter->OnSecondaryAction.RemoveDynamic(this, &UTP_WeaponComponent::OnSecondaryPressed);
		}
		if (FPSCharacter->OnSecondaryActionRelease.Contains(this, FName(TEXT("OnSecondaryReleased"))))
		{
			FPSCharacter->OnSecondaryActionRelease.RemoveDynamic(this, &UTP_WeaponComponent::OnSecondaryReleased);
		}

		if (FPSCharacter->OnReload.Contains(this, FName(TEXT("Reload"))))
		{
			FPSCharacter->OnReload.RemoveDynamic(this, &UTP_WeaponComponent::Reload);
		}

		// Reset secondary firing changes if secondary fire was held down. Kind of hack and could have unintended behavior.
		OnSecondaryReleased();

		// Stop automatic firing if Fire input is held down.
		if (IsAutomatic && AutoFireHandle.IsValid() && Character->GetWorldTimerManager().IsTimerActive(AutoFireHandle))
		{
			Character->GetWorldTimerManager().ClearTimer(AutoFireHandle);
		}
	}
}

void UTP_WeaponComponent::PlayFireAnimation()
{
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = FPSCharacter->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

