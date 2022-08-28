// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWeaponLeadingComponent.h"
#include "Kismet/KismetMathLibrary.h"

FRotator UEnemyWeaponLeadingComponent::GetProjectileRotation() const
{
	FVector PlayerVelocity = PlayerCharacter->GetVelocity(); // Player's velocity
	FVector DistanceVector = PlayerCharacter->GetActorLocation() - GetOwner()->GetActorLocation(); // Vector between Weapon and Player locations.
	FVector DistanceVelocityPlaneVector = PlayerVelocity.Cross(DistanceVector); // Vector normal to the plane created by the Player's velocity vector and the Distance Vector.
	// The angle between the Player's movement vector AND the vector from the Player to this WeaponComponent
	double MovementDistanceAngle = UKismetMathLibrary::DegAcos(PlayerVelocity.Dot(DistanceVector) / (PlayerVelocity.Length() * DistanceVector.Length()));
	float AngleToShoot = -1.0f * UKismetMathLibrary::DegAsin((PlayerVelocity.Length() * UKismetMathLibrary::DegSin(MovementDistanceAngle)) / ProjectileInitialSpeed);

	// Rotate the Distance vector by the calculated angle around the plane vector.
	FVector VectorToShoot = UKismetMathLibrary::RotateAngleAxis(DistanceVector, AngleToShoot, DistanceVelocityPlaneVector);

	return VectorToShoot.Rotation();
}
