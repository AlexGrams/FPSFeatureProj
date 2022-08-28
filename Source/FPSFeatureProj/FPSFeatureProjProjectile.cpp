// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSFeatureProjProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "HealthComponent.h"

AFPSFeatureProjProjectile::AFPSFeatureProjProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSFeatureProjProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
}

void AFPSFeatureProjProjectile::SetProperties_Implementation(float fDamage, AController* aPlayerController, AActor* aShooter, float InitialSpeed)
{
	Damage = fDamage;
	PlayerController = aPlayerController;
	Shooter = aShooter;

	// Set Projectile speed if InitialSpeed is non-negative. Else, use default Projectile speed value.
	if (InitialSpeed >= 0.0f)
	{
		FVector InitialVelocity = FVector(InitialSpeed, 0.0f, 0.0f);
		ProjectileMovement->MaxSpeed = InitialSpeed;
		ProjectileMovement->SetVelocityInLocalSpace(InitialVelocity);
	}
}

void AFPSFeatureProjProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Damage other object if it contains a HealthComponent
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherActor->FindComponentByClass(UHealthComponent::StaticClass()) != nullptr)
	{
		float TakenDamage = 0.0f;	// Actual damage that was applied
		TSubclassOf<UDamageType> DamageTypeClass = UDamageType::StaticClass();

		if (Damage != 0.f)
		{
			// make sure we have a good damage type
			TSubclassOf<UDamageType> const ValidDamageTypeClass = DamageTypeClass ? DamageTypeClass : TSubclassOf<UDamageType>(UDamageType::StaticClass());
			FDamageEvent DamageEvent(ValidDamageTypeClass);
			TakenDamage = OtherActor->TakeDamage(Damage, DamageEvent, PlayerController, Shooter);
		}

		Destroy();
	}

	// Only add impulse and destroy projectile if we hit a physics object
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
			Destroy();
		}
		// Destroy this projectile if it can't bounce off other objects
		else if (!bCanBounce)
		{
			Destroy();
		}
	}
}