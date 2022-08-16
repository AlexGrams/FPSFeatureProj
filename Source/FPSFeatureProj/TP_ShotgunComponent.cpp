// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_ShotgunComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Custom collision channels (For future reference. Some might not be used.)
#define ECC_Projectile			ECC_GameTraceChannel1
#define ECC_EnemyProjectile		ECC_GameTraceChannel2
#define ECC_Enemy				ECC_GameTraceChannel3

UTP_ShotgunComponent::UTP_ShotgunComponent()
{
	// Shotgun weapon default settings
	NumPellets = 50;
	SpreadHalfAngleDegrees = 10.0f;
	HitscanRange = 500.0f;
}

void UTP_ShotgunComponent::Fire()
{
	if (!IsValid(Character) || Character == nullptr || Character->GetController() == nullptr || CurrentAmmo <= 0)
	{
		return;
	}

	CurrentAmmo -= 1;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Shotgun fired")));
	//FVector PelletDirection = FVector::Zero();

	// TODO: For testing: draws the hitscan linetraces
	GetWorld()->DebugDrawTraceTag = FName(TEXT("Hitscan"));

	for (int i = 0; i < NumPellets; i++)
	{
		Super::FireHitscanWeapon();
		//// FVector RandomUnitVectorInConeInDegrees(FVector ConeDir, float ConeHalfAngleInDegrees)
		//PelletDirection = FMath::VRandCone(GetHitscanDirection(), FMath::DegreesToRadians(SpreadHalfAngleDegrees));
		////KismetMathLibrary::RandomUnitVectorInConeInDegrees(, SpreadHalfAngleDegrees);

		////GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *PelletDirection.ToCompactString()));

		//// Firing Shotgun

		//FHitResult OutHit = FHitResult();
		//FVector Start = GetHitscanStart();
		//FVector End = Start + HitscanRange * PelletDirection;

		//// List of types that hitscan bullet can hit
		//FCollisionObjectQueryParams ObjectQueryParams = FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic) |
		//	ECC_TO_BITFIELD(ECC_Pawn) | ECC_TO_BITFIELD(ECC_PhysicsBody) | ECC_TO_BITFIELD(ECC_Enemy));
		//// Line Trace parameters
		//FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("Hitscan")), true, Character);
		//bool Result = GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, ObjectQueryParams, Params);

		//if (Result)
		//{
		//	// Actual damage that was applied
		//	float TakenDamage = 0.0f;

		//	// TODO: Find best place to set these
		//	AActor* DamagedActor = OutHit.GetActor();
		//	AController* EventInstigator = Character->GetController();
		//	AActor* DamageCauser = Character;
		//	TSubclassOf<UDamageType> DamageTypeClass = UDamageType::StaticClass();

		//	if (!DamagedActor)
		//	{
		//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Unknown hit object")));
		//	}
		//	else if (Damage != 0.f)
		//	{
		//		// make sure we have a good damage type
		//		TSubclassOf<UDamageType> const ValidDamageTypeClass = DamageTypeClass ? DamageTypeClass : TSubclassOf<UDamageType>(UDamageType::StaticClass());
		//		FDamageEvent DamageEvent(ValidDamageTypeClass);
		//		TakenDamage = DamagedActor->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		//	}
		//}
	}

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	PlayFireAnimation();
}

FVector UTP_ShotgunComponent::GetHitscanDirection() const
{
	return FMath::VRandCone(Super::GetHitscanDirection(), FMath::DegreesToRadians(SpreadHalfAngleDegrees));
}
