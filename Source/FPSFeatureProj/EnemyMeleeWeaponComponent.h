// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyWeaponComponent.h"
#include "EnemyMeleeWeaponComponent.generated.h"

class UShapeComponent;

/**
 * Component for Enemies with a melee attack. Uses override of Fire() function to check for collisions with a hitbox.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FPSFEATUREPROJ_API UEnemyMeleeWeaponComponent : public UEnemyWeaponComponent
{
	GENERATED_BODY()

protected:
	// --- Variables 

	// A collider for the damaging area of this melee weapon
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	UShapeComponent* Hitbox;

public:
	// --- Functions ---

	// Check for collisions with a melee hitbox
	virtual void Fire() override;
};
