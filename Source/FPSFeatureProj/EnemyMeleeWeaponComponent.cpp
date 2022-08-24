// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMeleeWeaponComponent.h"
#include "FPSFeatureProjCharacter.h"
#include "Components/ShapeComponent.h"


void UEnemyMeleeWeaponComponent::Fire()
{
	if (!IsValid(HitboxComponent.GetComponent(GetOwner())))
	{
		return;
	}

	UShapeComponent* Hitbox = Cast<UShapeComponent>(HitboxComponent.GetComponent(GetOwner()));

	if (!IsValid(Hitbox))
	{
		return;
	}

	// Damage the Player (or Player-related objects) if they are overlapping with the hitbox
	TArray<AActor*> HitActors;

	Hitbox->GetOverlappingActors(HitActors, AFPSFeatureProjCharacter::StaticClass());
	if (HitActors.Num() > 0)
	{
		for (AActor* Target : HitActors)
		{
			DamageTarget(Target);
		}
	}
}
