// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWeaponLeadingComponent.h"
#include "Kismet/KismetMathLibrary.h"

FRotator UEnemyWeaponLeadingComponent::GetProjectileRotation() const
{
	// DegSin()
	// DegAsin()

	float ProjectileSpeed = 1000.0f; // TODO: Make settable/accessible from this component
	FVector PV = PlayerCharacter->GetVelocity(); // Player's velocity
	FVector DistanceVector = PlayerCharacter->GetActorLocation() - GetOwner()->GetActorLocation(); // Vector between Weapon and Player locations.
	FVector DistanceVelocityPlaneVector = PV.Cross(DistanceVector); // Vector normal to the plane created by the Player's velocity vector and the Distance Vector.
	// The angle between the Player's movement vector AND the vector from the Player to this WeaponComponent
	double MovementDistanceAngle = UKismetMathLibrary::DegAcos(PV.Dot(DistanceVector) / (PV.Length() * DistanceVector.Length()));
	double TestAngle = UKismetMathLibrary::DegAsin(DistanceVelocityPlaneVector.Length() / (PV.Length() * DistanceVector.Length())); // TODO: Valeus are the same. Choose to use one or the other.
	
	float AngleToShoot = -1.0f * UKismetMathLibrary::DegAsin((PV.Length() * UKismetMathLibrary::DegSin(MovementDistanceAngle)) / ProjectileSpeed);

	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, FString::Printf(TEXT("Comparing: %f / %f"), MovementDistanceAngle, TestAngle));
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, FString::Printf(TEXT("Vectors: %s / %s"), *PV.ToString(), *DistanceVector.ToString()));
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, FString::Printf(TEXT("Angle to fire at: %f / %f / %f / %f / %f"), AngleToShoot, MovementDistanceAngle, PV.Dot(DistanceVector), PV.Length(), DistanceVector.Length()));

	FVector VectorToShoot = UKismetMathLibrary::RotateAngleAxis(DistanceVector, AngleToShoot, DistanceVelocityPlaneVector);

	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, FString::Printf(TEXT("Final shot rotation: %s"), *VectorToShoot.Rotation().ToString()));

	return VectorToShoot.Rotation();
}