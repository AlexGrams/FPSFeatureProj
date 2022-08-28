// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyWeaponComponent.h"
#include "EnemyWeaponLeadingComponent.generated.h"

/**
 * Smarter Enemy Weapon that will aim ahead of (or lead) the Character if they are moving.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FPSFEATUREPROJ_API UEnemyWeaponLeadingComponent : public UEnemyWeaponComponent
{
	GENERATED_BODY()

protected:
	// --- Functions ---

	// Calculates a rotation to hit the Player if they continue moving in the same direction as this frame. 
	// ProjectileInitialSpeed should be non-negative for this to work.
	virtual FRotator GetProjectileRotation() const override;
};
