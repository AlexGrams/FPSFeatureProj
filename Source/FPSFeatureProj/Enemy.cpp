// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/ChildActorComponent.h"
#include "EnemyWeaponComponent.h"
#include "CritDamageType.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CritHitMultiplier = 2.0f;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	for (FComponentReference& Weapon : WeaponChildActors)
	{
		if (IsValid(Weapon.GetComponent(this)))
		{
			Weapons.Add(Cast<UChildActorComponent>(Weapon.GetComponent(this))->GetChildActor());
			WeaponComponents.Add(Cast<UEnemyWeaponComponent>(Weapons.Last()->GetComponentByClass(UEnemyWeaponComponent::StaticClass())));
		}
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Account for critical hit damage
	if (DamageEvent.DamageTypeClass == UCritDamageType::StaticClass())
	{
		DamageAmount *= CritHitMultiplier;
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AEnemy::FireWeapons()
{
	for (auto& EnemyWeaponComponent : WeaponComponents)
	{
		if (!IsValid(EnemyWeaponComponent))
		{
			continue;
		}

		if (EnemyWeaponComponent->bIsAutomatic)
		{
			EnemyWeaponComponent->StartAutoFire();
		}
		else
		{
			EnemyWeaponComponent->Fire();
		}
	}
}

void AEnemy::EndFiringWeapons()
{
	for (auto& EnemyWeaponComponent : WeaponComponents)
	{
		if (EnemyWeaponComponent->bIsAutomatic)
		{
			EnemyWeaponComponent->EndAutoFire();
		}
	}
}
