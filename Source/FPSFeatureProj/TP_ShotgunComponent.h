// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TP_WeaponComponent.h"
#include "TP_ShotgunComponent.generated.h"

/**
 * Component for shotguns the Player can use. When fired, creates multiple raycasts in a cone shape.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FPSFEATUREPROJ_API UTP_ShotgunComponent : public UTP_WeaponComponent
{
	GENERATED_BODY()
	
public:

	UTP_ShotgunComponent();

	// Creates many raycasts in a cone shape in front of the Player
	virtual void Fire() override;

protected:
	// --- Variables ---

	// How many pellets (line traces) are created each time the shotgun is fired.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	int NumPellets;

	// The half angle (angle between center of cone and side) of the shotgun spread in degrees.
	float SpreadHalfAngleDegrees;

	// Get the direction of a hitscan line trace
	virtual FVector GetHitscanDirection() const override;
};
