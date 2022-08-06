// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "HealthInterface.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	DefaultHealth = 100.0f;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
	Health = DefaultHealth;
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	// Can't take damage if dead already. Could change to allow overkill by moving check inward.
	if (!IsDead)
	{
		IHealthInterface* Interface = Cast<IHealthInterface>(Owner);

		Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
		if (Owner->Implements<UHealthInterface>())
		{
			Interface->Execute_DamageTaken(Owner, DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
		}

		if (Health <= 0)
		{
			// Check if parent has HealthInterface, then call HealthDepleted.

			if (Owner->Implements<UHealthInterface>())
			{
				IsDead = true;
				Interface->Execute_HealthDepleted(Owner);
			}
			else
			{
				// Warn about not having a health depleted function despite
				// having health interface.
			}
		}
	}
}

